#include <inc/memlayout.h>
#include "shared_memory_manager.h"

#include <inc/mmu.h>
#include <inc/error.h>
#include <inc/string.h>
#include <inc/assert.h>
#include <inc/queue.h>
#include <inc/environment_definitions.h>

#include <kern/proc/user_environment.h>
#include <kern/trap/syscall.h>
#include "kheap.h"
#include "memory_manager.h"

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//
struct Share* get_share(int32 ownerID, char* name);

//===========================
// [1] INITIALIZE SHARES:
//===========================
//Initialize the list and the corresponding lock
void sharing_init()
{
#if USE_KHEAP
	LIST_INIT(&AllShares.shares_list) ;
	init_spinlock(&AllShares.shareslock, "shares lock");
#else
	panic("not handled when KERN HEAP is disabled");
#endif
}

//==============================
// [2] Get Size of Share Object:
//==============================
int getSizeOfSharedObject(int32 ownerID, char* shareName)
{
	//[PROJECT'24.MS2] DONE
	// This function should return the size of the given shared object
	// RETURN:
	//	a) If found, return size of shared object
	//	b) Else, return E_SHARED_MEM_NOT_EXISTS
    acquire_spinlock(&AllShares.shareslock);
	struct Share* ptr_share = get_share(ownerID, shareName);
	if (ptr_share == NULL){
		release_spinlock(&AllShares.shareslock);
		return E_SHARED_MEM_NOT_EXISTS;
	}
	else{
		release_spinlock(&AllShares.shareslock);
		return ptr_share->size;
	}
	release_spinlock(&AllShares.shareslock);
	return 0;
}

//===========================================================


//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
//===========================
// [1] Create frames_storage:
//===========================
// Create the frames_storage and initialize it by 0
inline struct FrameInfo** create_frames_storage(int numOfFrames)
{
	//TODO: [PROJECT'24.MS2 - #16] [4] SHARED MEMORY - create_frames_storage()

	struct FrameInfo** storedFrames = kmalloc(numOfFrames * sizeof(struct FrameInfo*));
	if (!storedFrames) return NULL; // Handle allocation failure.

	memset(storedFrames, 0, numOfFrames * sizeof(struct FrameInfo*));

	return storedFrames;


}

struct Share* create_share(int32 ownerID, char* shareName, uint32 size, uint8 isWritable)
{
	  //TODO: [PROJECT'24.MS2 - #16] [4] SHARED MEMORY - create_share()

	  void* ans = kmalloc(sizeof (struct Share));
	    if (!ans){
	    	return NULL;
	    }

	    struct Share* firstShare = (struct Share*) ans;
	    firstShare->ownerID = ownerID;
	    strcpy(firstShare->name, shareName);
	    firstShare->size = size;
	    firstShare->isWritable = isWritable;


	    int32 mask = 0x7FFFFFFF;
	    firstShare->ID = (uint32) ans & mask;

	    firstShare->references = 1;

	    uint32 numOfFrames = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;

	    struct FrameInfo** frames_stored = create_frames_storage(numOfFrames);
	    if (!frames_stored) {
	        kfree(ans); // Clean up allocated memory
	        return NULL;
	    }
	    firstShare->framesStorage = frames_stored;

	    return firstShare;
}

struct Share* get_share(int32 ownerID, char* name) {
    if (name == NULL) {
        return NULL;
    }
    struct Share* share = NULL;
    LIST_FOREACH(share, &AllShares.shares_list) {
        // Compare ownerID and name
        if (share->ownerID == ownerID && strcmp(share->name, name) == 0) {
            return share;
        }
    }
    return NULL;
}



//=========================
// [4] Create Share Object:
//=========================
int createSharedObject(int32 ownerID, char* shareName, uint32 size, uint8 isWritable, void* virtual_address) {
    acquire_spinlock(&AllShares.shareslock);
	struct Env* myenv = get_cpu_proc();
    struct Share* share = get_share(ownerID, shareName);
    if (share != NULL) {
    	release_spinlock(&AllShares.shareslock);
        return E_SHARED_MEM_EXISTS;
    }

    struct Share* new_to_alloc = create_share(ownerID, shareName, size, isWritable);
    if (new_to_alloc == NULL) {
        release_spinlock(&AllShares.shareslock);
    	return E_NO_SHARE;
    }
    new_to_alloc->startsddrc =virtual_address;

    LIST_INSERT_TAIL(&(AllShares.shares_list), new_to_alloc);

    uint32 required_pages = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;
    for (uint32 i = 0; i < required_pages; i++) {
    	struct FrameInfo* frame;
    	uint32 al = allocate_frame(&frame);
    	if (al == E_NO_MEM) {
    		kfree(new_to_alloc);
    		release_spinlock(&AllShares.shareslock);
    		return E_NO_MEM;
    	}

        // Mapping frames to the virtual address space
        uint32 addr = (uint32)virtual_address + i * PAGE_SIZE;
        uint32 perm_flags =  PERM_USER |PERM_WRITEABLE|PERM_AVAILABLE;
        map_frame(myenv->env_page_directory, frame, addr, perm_flags);
        new_to_alloc->framesStorage[i] = frame;
    }
    release_spinlock(&AllShares.shareslock);
    return new_to_alloc->ID;
}
//======================
// [5] Get Share Object:
//======================
int getSharedObject(int32 ownerID, char* shareName, void* virtual_address)
{
    //TODO: [PROJECT'24.MS2 - #21] [4] SHARED MEMORY [KERNEL SIDE] - getSharedObject()
    acquire_spinlock(&AllShares.shareslock);
	struct Share*mariam = get_share( ownerID, shareName);
    if(mariam == NULL){
    	release_spinlock(&AllShares.shareslock);
    	return E_SHARED_MEM_NOT_EXISTS;
    }
    mariam->references+=1;
    mariam->startsddrg=virtual_address;
    struct Env* myenv = get_cpu_proc(); //The calling environment
    uint32 numpages = ROUNDUP(mariam->size,PAGE_SIZE) / PAGE_SIZE;
    for (int i = 0; i < numpages ; i++) {
        struct FrameInfo* frame = mariam->framesStorage[i];
        if (mariam->isWritable) {
            map_frame(myenv->env_page_directory, frame,(uint32) virtual_address, PERM_PRESENT | PERM_WRITEABLE | PERM_USER);//fn explanation
        }
        else {
            map_frame(myenv->env_page_directory, frame,(uint32) virtual_address,  PERM_PRESENT  | PERM_USER );//read only

        }
        virtual_address += PAGE_SIZE;
    }
    release_spinlock(&AllShares.shareslock);
    return mariam->ID;
}

//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//==========================
// [B1] Delete Share Object:
//==========================
void free_share(struct Share* ptrShare) {
    LIST_REMOVE(&AllShares.shares_list, ptrShare);
    kfree(ptrShare->framesStorage);
    kfree(ptrShare);
}


//========================
// [B2] Free Share Object:
//========================
int is_used(uint32 *ptr_page_table)
{
	for (int i = 0; i < 1024; i++)
	{
		if (ptr_page_table[i])
			return 1;
	}
	return 0;
}
int freeSharedObject(int32 sharedObjectID, void *startVA) {
	acquire_spinlock(&AllShares.shareslock);
	sharedObjectID=get_id_by_virtual_address(startVA);
	struct Env *myenv = get_cpu_proc();

	uint32 virtual_address = (uint32)startVA;

	bool found=0;
    struct Share* share = NULL;
    LIST_FOREACH(share, &AllShares.shares_list) {
        if (share->ID == sharedObjectID) {
        	found=1;
            break;
        }
    }
    if(!found){
    	release_spinlock(&AllShares.shareslock);
    	return 0;
    }
	uint32 startaddr = ROUNDDOWN(virtual_address, PAGE_SIZE),
    endaddr = ROUNDUP(virtual_address + share->size, PAGE_SIZE);
	while (startaddr < endaddr)
	{
		unmap_frame(myenv->env_page_directory, startaddr);
		startaddr += PAGE_SIZE;
	}

	//free table if is empty
	startaddr = ROUNDDOWN(virtual_address, PAGE_SIZE),
    endaddr = ROUNDUP(virtual_address + share->size, PAGE_SIZE);
	while (startaddr < endaddr)
	{
		uint32 *ptr_page_table = NULL;
		int ret = get_page_table(myenv->env_page_directory, startaddr, &ptr_page_table);
		pt_set_page_permissions(myenv->env_page_directory,startaddr,0,PERM_USER |PERM_WRITEABLE|PERM_AVAILABLE);
		pt_clear_page_table_entry(myenv->env_page_directory, startaddr);
		if (ret != TABLE_NOT_EXIST && !is_used(ptr_page_table))
		{
			kfree((void *)ptr_page_table);
			pd_clear_page_dir_entry(myenv->env_page_directory, startaddr);
		}
		startaddr += PAGE_SIZE;
	}
	share->references--;
	if (share->references == 0)
		free_share(share);

	tlbflush();
	release_spinlock(&AllShares.shareslock);
	return 0;
}
//helper function
int get_id_by_virtual_address(void* virtual_address){
	struct Share* share = NULL;
	LIST_FOREACH(share, &AllShares.shares_list) {
		if (share->startsddrc==virtual_address||share->startsddrg==virtual_address) {
			return share->ID;
		}
	}
	return 0;
}

