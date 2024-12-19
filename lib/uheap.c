#include <inc/lib.h>

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//=============================================
// [1] CHANGE THE BREAK LIMIT OF THE USER HEAP:
//=============================================
/*2023*/
void* sbrk(int increment)
{
	return (void*) sys_sbrk(increment);
}

//=================================
// [2] ALLOCATE SPACE IN USER HEAP:
//=================================
int user_heap_pages[NUM_OF_UHEAP_PAGES];
void* malloc(uint32 size)
{
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	if (size == 0) return NULL ;
	//==============================================================
	//TODO: [PROJECT'24.MS2 - #12] [3] USER HEAP [USER SIDE] - malloc()
    if (size <= DYN_ALLOC_MAX_BLOCK_SIZE) {
        return alloc_block_FF(size);
    }
    if (!sys_isUHeapPlacementStrategyFIRSTFIT()) {
        return NULL;
    }

    // Handle larger allocations
    uint32 start = (myEnv->hardLimit + PAGE_SIZE - USER_HEAP_START) / PAGE_SIZE;
    unsigned int numPages = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE; // Calculate required pages
    uint32 numFreePages = 0;
    void* startAddress = NULL;
    uint32 start_idx = 0;
    for (uint32 addr = start; addr < NUM_OF_UHEAP_PAGES; addr++) {
            if (user_heap_pages[addr] != 0) { // Occupied page
                numFreePages = 0;
                addr += user_heap_pages[addr] - 1; // Skip occupied block
            } else { // Free page
                if (numFreePages == 0) {
                    start_idx = addr; // Mark start of the block
                }
                numFreePages++;
                if (numFreePages == numPages) { // Found suitable block
                	user_heap_pages[start_idx] = numPages; // Mark block as used
                    void* startAddress = (void*)(start_idx * PAGE_SIZE) + USER_HEAP_START;
                    sys_allocate_user_mem((uint32)startAddress, size);
                    return (void *)startAddress; // Return starting address
                }
            }
    }

    return NULL;

}

//=================================
// [3] FREE SPACE FROM USER HEAP:
//=================================
void free(void* virtual_address) {
    uint32 VirtualAddress = (uint32)virtual_address;

    // Check if the address is within the user heap limits
    if (VirtualAddress >= USER_HEAP_START && VirtualAddress < myEnv->hardLimit) {
        free_block(virtual_address); // Handle small allocations
    }
    else if (VirtualAddress >= (myEnv->hardLimit + PAGE_SIZE) && VirtualAddress < USER_HEAP_MAX) {
        VirtualAddress = ROUNDDOWN(VirtualAddress, PAGE_SIZE);
        uint32 pagenum = (VirtualAddress -USER_HEAP_START) / PAGE_SIZE;

        uint32 found_numofpages = user_heap_pages[pagenum];
        if (found_numofpages == 0) {
            panic("Invalid Address!!");
        }

        uint32 size = found_numofpages * PAGE_SIZE;
        user_heap_pages[pagenum] = 0;// Mark pages as free
        sys_free_user_mem(VirtualAddress, size); // Free the memory

    }
}

//=================================
// [4] ALLOCATE SHARED VARIABLE:
//=================================
void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable) {
    if (!sys_isUHeapPlacementStrategyFIRSTFIT()) {
        return NULL;
    }

    uint32 start = (myEnv->hardLimit + PAGE_SIZE - USER_HEAP_START) / PAGE_SIZE;
    unsigned int numPages = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;
    uint32 numFreePages = 0, start_idx = 0;

    for (uint32 addr = start; addr < NUM_OF_UHEAP_PAGES; addr++) {
        if (user_heap_pages[addr] == 0) {
            if (numFreePages == 0) {
                start_idx = addr;
            }
            numFreePages++;
            if (numFreePages == numPages) {
                user_heap_pages[start_idx] = numPages;
                void* startAddress = (void*)(start_idx * PAGE_SIZE) + USER_HEAP_START;
                //sys_allocate_user_mem((uint32)startAddress, size);
                int res = sys_createSharedObject(sharedVarName, size, isWritable, startAddress);
                if (res < 0) {
                    user_heap_pages[start_idx] = 0;// Rollback on failure
                    return NULL;
                }

                return startAddress;
            }
        } else {
            numFreePages = 0;
            addr += user_heap_pages[addr] - 1;
        }
    }

    return NULL; // No suitable block found
}

//========================================
// [5] SHARE ON ALLOCATED SHARED VARIABLE:
//========================================
void* sget(int32 ownerEnvID, char* sharedVarName) {
    // Get the size of the shared object
    int size = sys_getSizeOfSharedObject(ownerEnvID, sharedVarName);
    if (size < 0) {
        return NULL;
    }

    // Align the start address to a 4KB boundary
    uint32 start = (myEnv->hardLimit + PAGE_SIZE - USER_HEAP_START) / PAGE_SIZE;
    unsigned int numPages = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;
    uint32 numFreePages = 0, start_idx = 0;

    // Find a suitable range in the heap
    for (uint32 addr = start; addr < NUM_OF_UHEAP_PAGES; addr++) {
        if (user_heap_pages[addr] == 0) {
            if (numFreePages == 0) {
                start_idx = addr;
            }
            numFreePages++;
            if (numFreePages == numPages) {
                user_heap_pages[start_idx] = numPages;
                void* startAddress = (void*)(start_idx * PAGE_SIZE) + USER_HEAP_START;


                sys_allocate_user_mem((uint32)startAddress, size);
                int res = sys_getSharedObject(ownerEnvID, sharedVarName, startAddress);

                if (res < 0) {
                    user_heap_pages[start_idx] = 0; // Rollback
                    return NULL;
                }
                return startAddress;
            }
        } else {
            numFreePages = 0;
            addr += user_heap_pages[addr] - 1;
        }
    }

    return NULL;
}

//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//=================================
// FREE SHARED VARIABLE:
//=================================
//	This function frees the shared variable at the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from main memory then switch back to the user again.
//
//	use sys_freeSharedObject(...); which switches to the kernel mode,
//	calls freeSharedObject(...) in "shared_memory_manager.c", then switch back to the user mode here
//	the freeSharedObject() function is empty, make sure to implement it.

void sfree(void* virtual_address)
{
	//TODO: [PROJECT'24.MS2 - BONUS#4] [4] SHARED MEMORY [USER SIDE] - sfree()
	//uint32 Virtual_address = ROUNDDOWN(virtual_address,PAGE_SIZE);
	int32 mask = 0x7FFFFFFF; // Mask for clearing the MSB
	uint32 ID = (uint32) virtual_address & mask;
	sys_freeSharedObject(ID,virtual_address);
	uint32 start = ((uint32) virtual_address - USER_HEAP_START) / PAGE_SIZE;
	user_heap_pages[start]=0;
}


//=================================
// REALLOC USER SPACE:
//=================================
//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_move_user_mem(...)
//		which switches to the kernel mode, calls move_user_mem(...)
//		in "kern/mem/chunk_operations.c", then switch back to the user mode here
//	the move_user_mem() function is empty, make sure to implement it.
void *realloc(void *virtual_address, uint32 new_size)
{
	//[PROJECT]
	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");
	return NULL;

}


//==================================================================================//
//========================== MODIFICATION FUNCTIONS ================================//
//==================================================================================//

void expand(uint32 newSize)
{
	panic("Not Implemented");

}
void shrink(uint32 newSize)
{
	panic("Not Implemented");

}
void freeHeap(void* virtual_address)
{
	panic("Not Implemented");

}
