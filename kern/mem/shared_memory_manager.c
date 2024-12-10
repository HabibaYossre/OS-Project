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
	//
	/*int x=0;
		if (!(holding_spinlock(&AllShares.shareslock))){
			    acquire_spinlock(&AllShares.shareslock);
			    x=1;

			}*/
   acquire_spinlock(&AllShares.shareslock);
	struct Share* ptr_share = get_share(ownerID, shareName);
	if (ptr_share == NULL){

		//if (x){
			release_spinlock(&AllShares.shareslock);

		//}
		return E_SHARED_MEM_NOT_EXISTS;
	}
	else{
		//if (x){
			release_spinlock(&AllShares.shareslock);

		//}
		return ptr_share->size;
	}
	//if (x){
		release_spinlock(&AllShares.shareslock);

	//}
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
   // acquire_spinlock(&AllShares.shareslock);
    if (name == NULL) {
        return NULL;
    }
    struct Share* share = NULL;
    LIST_FOREACH(share, &AllShares.shares_list) {
        // Compare ownerID and name
        if (share->ownerID == ownerID && strcmp(share->name, name) == 0) {
            //release_spinlock(&AllShares.shareslock);
            return share;
        }
    }
    //release_spinlock(&AllShares.shareslock); // Ensure lock is released here
    return NULL;
}



//=========================
// [4] Create Share Object:
//=========================
int createSharedObject(int32 ownerID, char* shareName, uint32 size, uint8 isWritable, void* virtual_address) {
	/*int x=0;
		if (!(holding_spinlock(&AllShares.shareslock))){
		    acquire_spinlock(&AllShares.shareslock);
		    x=1;
		}*/
    acquire_spinlock(&AllShares.shareslock);
	struct Env* myenv = get_cpu_proc();
    struct Share* share = get_share(ownerID, shareName);
    if (share != NULL) {
    	//if (x){
    	         release_spinlock(&AllShares.shareslock);

    	         //  }
        return E_SHARED_MEM_EXISTS;
    }

    struct Share* new_to_alloc = create_share(ownerID, shareName, size, isWritable);
    if (new_to_alloc == NULL) {
    	//if (x){
    		release_spinlock(&AllShares.shareslock);

    	//}
    	return E_NO_SHARE;
    }
    new_to_alloc->startsddr =virtual_address;

    //acquire_spinlock(&AllShares.shareslock);
    LIST_INSERT_TAIL(&(AllShares.shares_list), new_to_alloc);
    //release_spinlock(&AllShares.shareslock);

    uint32 required_pages = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;
    for (uint32 i = 0; i < required_pages; i++) {
    	struct FrameInfo* frame;
    	uint32 al = allocate_frame(&frame);
    	if (al == E_NO_MEM) {
    		kfree(new_to_alloc);
    		//if (x){
    			release_spinlock(&AllShares.shareslock);

    		//}
    		return E_NO_MEM;
    	}

        // Mapping frames to the virtual address space
        uint32 addr = (uint32)virtual_address + i * PAGE_SIZE;
        uint32 perm_flags =  PERM_USER |PERM_WRITEABLE|PERM_AVAILABLE;
        map_frame(myenv->env_page_directory, frame, addr, perm_flags);
        new_to_alloc->framesStorage[i] = frame;
    }
    //if (x){
             release_spinlock(&AllShares.shareslock);

               //}
    return new_to_alloc->ID;
}
//======================
// [5] Get Share Object:
//======================
int getSharedObject(int32 ownerID, char* shareName, void* virtual_address)
{
    //TODO: [PROJECT'24.MS2 - #21] [4] SHARED MEMORY [KERNEL SIDE] - getSharedObject()
	/*int x=0;
			if (!(holding_spinlock(&AllShares.shareslock))){
			    acquire_spinlock(&AllShares.shareslock);
			    x=1;
			}*/
    acquire_spinlock(&AllShares.shareslock);
	struct Share*mariam = get_share( ownerID, shareName);
    if(mariam == NULL){
    	//if (x){
    	    	         release_spinlock(&AllShares.shareslock);

    	    	           //}
    	return E_SHARED_MEM_NOT_EXISTS;
    }
    mariam->references+=1;
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
   // if (x){
    	release_spinlock(&AllShares.shareslock);

    //}
    return mariam->ID;
}

//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//==========================
// [B1] Delete Share Object:
//==========================
//delete the given shared object from the "shares_list"
//it should free its framesStorage and the share object itself
void free_share(struct Share* ptrShare)
{/*
	//TODO: [PROJECT'24.MS2 - BONUS#4] [4] SHARED MEMORY [KERNEL SIDE] - free_share()
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("free_share is not implemented yet");
	//Your Code is Here...
	acquire_spinlock(&AllShares.shareslock);
	LIST_REMOVE(&AllShares.shares_list,ptrShare);
	release_spinlock(&AllShares.shareslock);
	kfree((void*)(ptrShare->framesStorage));
	kfree((void*)ptrShare);
*/
	//free((void*)ptrShare);


}
//========================
// [B2] Free Share Object:
//========================

int freeSharedObject(int32 sharedObjectID, void *startVA) {

	return 0;
}


	/*
	sharedObjectID = get_id_by_virtual_address(startVA);
	cprintf("after masking received id: %d\n", sharedObjectID);

	// Align startVA to the nearest page boundary
	//startVA = ROUNDDOWN(startVA, PAGE_SIZE);

	struct Env* myenv = get_cpu_proc();
	struct Share* share = NULL;
	struct Share* shareFound = NULL;

	// Locate the shared object
	acquire_spinlock(&AllShares.shareslock);
	 LIST_FOREACH(share, &AllShares.shares_list) {
	        if (share->ID == sharedObjectID) {
	        	shareFound = share;
	        	cprintf("*****************found*************\n");
	            break;
	        }
	    }
	release_spinlock(&AllShares.shareslock);
	cprintf("*****************RELEASED*************\n");

	if (shareFound == NULL) {
	    return 0; // Shared object not found
	}

	// Calculate the number of pages
	int size = shareFound->size;
	unsigned int numPages = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;

	// Iterate through the pages and release resources
	for (uint32 i = 0; i < numPages; i++) {
	    uint32 pageAddress = (uint32)startVA + i * PAGE_SIZE;

	  // pt_set_page_permissions(myenv->env_page_directory, pageAddress, 0, PERM_MARKED);
	  //  cprintf("*****************setPerms*************\n");
	    // Unmap the frame and clear permissions
	    unmap_frame(myenv->env_page_directory, pageAddress);
    	cprintf("*****************unmap*************\n");

	    pt_clear_page_table_entry(myenv->env_page_directory, pageAddress);
    	cprintf("*****************clear_page_table_entry*************\n");

	    // Check if the page table is no longer used
	    if (!pd_is_table_used(myenv->env_page_directory, pageAddress)) {
	    	uint32  res;
	    	uint32 * ptr_page_table= NULL;
	    	res = get_page_table(myenv->env_page_directory,pageAddress,&ptr_page_table);
	    	if (res == TABLE_NOT_EXIST){
	    		  continue;
	    		}
	    	kfree((void*)ptr_page_table);
	    	cprintf("*****************free_user_mem*************\n");

	    }
	}

	// Decrement the reference count
	shareFound->references--;

	// Free the shared object if no references remain
	if (shareFound->references == 0) {
	    free_share(shareFound);
	    tlbflush(); // Flush TLB to maintain consistency
	}

	return 0; // Indicate success
*/

    /*sharedObjectID = get_id_by_virtual_address(startVA);
    cprintf("after  maskingreseived id : %d\n  ", sharedObjectID);
    startVA = ROUNDDOWN(startVA,PAGE_SIZE);

    struct Env* myenv = get_cpu_proc();
    struct Share* share = NULL;
    struct Share* shareFound = NULL;
    acquire_spinlock(&AllShares.shareslock);
    LIST_FOREACH(share, &AllShares.shares_list) {
        if (share->ID == sharedObjectID) {
        	shareFound = share;
        	cprintf("*****************found*************\n");
            break;
        }
    }
    release_spinlock(&AllShares.shareslock);
    cprintf("*****************RELESED*************\n");

    if (shareFound == NULL) {
        return 0; // Shared object not found
    }
    int size = shareFound->size;
    unsigned int numPages = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;
    for (uint32 i = 0; i < numPages; i++) {
    	unmap_frame(myenv->env_page_directory, (uint32)startVA+i*PAGE_SIZE);
    	pt_set_page_permissions(myenv->env_page_directory,(uint32)startVA+i*PAGE_SIZE, 0, PERM_MARKED);
    	pt_clear_page_table_entry(myenv->env_page_directory, (uint32)startVA+i*PAGE_SIZE);
    	if ( pd_is_table_used(myenv->env_page_directory, (uint32)startVA+i*PAGE_SIZE)==0){
    		uint32 pageadd = (uint32)startVA+i*PAGE_SIZE;
    		free_user_mem(myenv, pageadd,PAGE_SIZE);
    	}
    	}
    shareFound->references--;
    if (shareFound->references == 0) {
        	free_share(share);
        	tlbflush(); // Flush TLB to maintain consistency

            }
        return E_SHARED_MEM_EXISTS;

    }*/
/*
   uint32 numOfFrames = ROUNDUP(share->size, PAGE_SIZE) / PAGE_SIZE;
   for (uint32 i = 0; i < numOfFrames; i++) {
       unmap_frame(myenv->env_page_directory, (uint32)startVA+i*PAGE_SIZE);
       cprintf("*****************unmap*************\n");
      if(share->framesStorage[i]->references==0){
       free_frame(share->framesStorage[i]);
       }
    	//cprintf("*****************freeframe*************\n");
       if (share->framesStorage[i]->references == 0) {
           // Clear the page table entry
           pt_clear_page_table_entry(myenv->env_page_directory, (uint32)startVA + i * PAGE_SIZE);
           cprintf("*****************clear_page_table_entry*************\n");

           // Check if the page table is still used
           if (!pd_is_table_used(myenv->env_page_directory, (uint32)startVA + i * PAGE_SIZE)) {
               // Retrieve the frame information
               uint32* ptr_page_table = NULL;
               struct frame_info;
               struct frame_info frame = get_frame_info(myenv->env_page_directory, (uint32)startVA + i * PAGE_SIZE, &ptr_page_table);

               if (frame != NULL) {
                   // Free the frame associated with the page table
                   free_frame(frame);
                   cprintf("*****************freeframe*************\n");
               }

               // Clear the page directory entry only after freeing the frame
               pd_clear_page_dir_entry(myenv->env_page_directory, (uint32)startVA + i * PAGE_SIZE);
           }
       }

   }

      shareFound->references--;
      cprintf("*****************ref--*************\n");
    // Free the shared object
    if (shareFound->references == 0) {
    	free_share(share);
    	tlbflush(); // Flush TLB to maintain consistency

        }
    return E_SHARED_MEM_EXISTS;
    */

int get_id_by_virtual_address(void* virtual_address){
        struct Share* share = NULL;
        acquire_spinlock(&AllShares.shareslock);
        LIST_FOREACH(share, &AllShares.shares_list) {
            // Compare ownerID and name
            if (share->startsddr==virtual_address) {
                release_spinlock(&AllShares.shareslock);
                return share->ID;
            }
        }
        release_spinlock(&AllShares.shareslock); // Ensure lock is released here
        return 0;
}

