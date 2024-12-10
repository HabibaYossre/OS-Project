#include "kheap.h"

#include <inc/memlayout.h>
#include <inc/dynamic_allocator.h>
#include "memory_manager.h"
/***************************************helper ms2**************************************/
uint32 Raheeq[NUM_OF_KHEAP_PAGES];
uint32 it;
int help;
/*************************************************************************************/
//Initialize the dynamic allocator of kernel heap with the given start address, size & limit

uint32 alloc_new_frames(uint32 startpoint, uint32 size) {
    for (uint32 itr = startpoint; itr < startpoint + size; itr += PAGE_SIZE) {
        struct FrameInfo* frame;
        uint32 ans = allocate_frame(&frame);
        if (ans == E_NO_MEM) {
            return E_NO_MEM; // Not enough memory
        }
        // Map the allocated frame to the current address
        map_frame(ptr_page_directory, frame, itr, PERM_WRITEABLE);
    }
    return 0; // Success
}

int initialize_kheap_dynamic_allocator(uint32 daStart, uint32 initSizeToAllocate, uint32 daLimit)
{
    //[PROJECT'24.MS2] [USER HEAP - KERNEL SIDE] initialize_kheap_dynamic_allocator
    // Write your code here, remove the panic and write your code

    startAdd=(uint32)daStart;
    hardLimit=(uint32)daLimit;
    sbr=startAdd+initSizeToAllocate;
    sbr=ROUNDUP(sbr,PAGE_SIZE);
    init_spinlock(&raheeq_lock, "kheap lock");
    /***************************************helper ms2**************************************/
    numoffreepages=(KERNEL_HEAP_MAX-hardLimit-PAGE_SIZE)/PAGE_SIZE;
    it=((hardLimit+PAGE_SIZE-KERNEL_HEAP_START)/PAGE_SIZE);
    help=0;
    /*************************************************************************************/
   // uint32 size=sbr-startAdd;
    if(startAdd>daLimit||startAdd+initSizeToAllocate>daLimit){
        return E_NO_MEM;
       // panic("i am heeereee");
    }
    uint32 ans=alloc_new_frames(startAdd,initSizeToAllocate);
    if(ans==E_NO_MEM)return E_NO_MEM;

   initialize_dynamic_allocator(startAdd,initSizeToAllocate);
   return 0 ; //success
}

void* sbrk(int numOfPages) {
    if (numOfPages == 0) {
        return (void*)sbr; // Return current segment break
    }

    uint32 old_sbr = sbr;
    uint32 new_sbr = sbr + (numOfPages*PAGE_SIZE);

    if (new_sbr < hardLimit && new_sbr >= startAdd) {
        sbr = new_sbr; // Update segment break
        if (alloc_new_frames(old_sbr, new_sbr - old_sbr) != 0) {
            panic("Failed to allocate new frames in sbrk");
        }
        return (void*)old_sbr; // Return the old segment break
    }

    //panic("Heap is Full Man!!!");
    return (void*)-1; // This line should not be reached
}

//TODO: [PROJECT'24.MS2 - BONUS#2] [1] KERNEL HEAP - Fast Page Allocator
void* allocate(void* startAddress,unsigned int numPages){
	bool lock_already_held = holding_spinlock(&MemFrameLists.mfllock);

		if (!lock_already_held)
		{
			acquire_spinlock(&MemFrameLists.mfllock);
		}

	for (uint32 i = 0; i < numPages; i++) {
	     struct FrameInfo* frame;
	     if (allocate_frame(&frame) == E_NO_MEM) {
	    	 if (!lock_already_held)
	    	 	{
	    	 		release_spinlock(&MemFrameLists.mfllock);
	    	 	}

	        return NULL; // Allocation failed
	     }
	     map_frame(ptr_page_directory, frame, (int)(startAddress + i * PAGE_SIZE), PERM_PRESENT | PERM_WRITEABLE);
	}
	if (!lock_already_held)
		{
			release_spinlock(&MemFrameLists.mfllock);
		}
	return (void*)1;
}

void* kmalloc(unsigned int size) {
	acquire_spinlock(&raheeq_lock);
	bool lock_already_held = holding_spinlock(&MemFrameLists.mfllock);

			if (!lock_already_held)
			{
				acquire_spinlock(&MemFrameLists.mfllock);
			}
    if (size==0){
    	if (!lock_already_held)
    			{
    				release_spinlock(&MemFrameLists.mfllock);
    			}
    	release_spinlock(&raheeq_lock);
        return NULL;
    }
    // Handle small allocations
    if (size <= DYN_ALLOC_MAX_BLOCK_SIZE) {
    	if (!lock_already_held)
    			{
    				release_spinlock(&MemFrameLists.mfllock);
    			}
    	release_spinlock(&raheeq_lock);
        return alloc_block_FF(size);
    }

    // Ensure the heap placement strategy is FIRST FIT
    if (!isKHeapPlacementStrategyFIRSTFIT()) {
    	if (!lock_already_held)
    			{
    				release_spinlock(&MemFrameLists.mfllock);
    			}
    	release_spinlock(&raheeq_lock);
        return NULL;
    }
    // Calculate required pages and ensure enough free pages
    unsigned int numPages = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;

    // Define search boundaries and variables
    uint32 start = (hardLimit + PAGE_SIZE - KERNEL_HEAP_START) / PAGE_SIZE;
    uint32 numFreePages = 0;
    uint32 start_idx = 0;
    void* startAddress=NULL;
   if (numoffreepages >= numPages && !help ) {

        // Calculate starting address
        startAddress = (void*)(it * PAGE_SIZE) + KERNEL_HEAP_START;
        if(allocate(startAddress,numPages)==NULL){
        	if (!lock_already_held)
        			{
        				release_spinlock(&MemFrameLists.mfllock);
        			}
        	release_spinlock(&raheeq_lock);
        	return NULL;
        }

        // Update metadata
        Raheeq[it] = numPages;
        it += numPages;// Move the iterator
        numoffreepages-=numPages;
        if (!lock_already_held)
        		{
        			release_spinlock(&MemFrameLists.mfllock);
        		}
        release_spinlock(&raheeq_lock);
        return startAddress;
    }


    // Find contiguous free pages using FIRST FIT
    for (uint32 addr = start; addr < NUM_OF_KHEAP_PAGES; addr++) {
        if (Raheeq[addr] != 0) { // Occupied page
            numFreePages = 0;
            addr += Raheeq[addr] - 1; // Skip occupied block
        } else { // Free page
            if (numFreePages == 0) {
                start_idx = addr; // Mark start of the block
            }
            numFreePages++;
            if (numFreePages == numPages) { // Found suitable block
                Raheeq[start_idx] = numPages; // Mark block as used
                void* startAddress = (void*)(start_idx * PAGE_SIZE) + KERNEL_HEAP_START;

                // Allocate and map frames
                if(allocate(startAddress,numPages)==NULL){
                	if (!lock_already_held)
                			{
                				release_spinlock(&MemFrameLists.mfllock);
                			}
                	release_spinlock(&raheeq_lock);
                    return NULL;
                }
                if (!lock_already_held)
                		{
                			release_spinlock(&MemFrameLists.mfllock);
                		}
                release_spinlock(&raheeq_lock);
                return startAddress; // Return starting address
            }
        }
    }
    if (!lock_already_held)
    		{
    			release_spinlock(&MemFrameLists.mfllock);
    		}
    release_spinlock(&raheeq_lock);
    // Allocation failed
    return NULL;
}


void kfree(void* virtual_address)
{
    //TODO: [PROJECT'24.MS2 - #04] [1] KERNEL HEAP - kfree
    // Write your code here, remove the panic and write your code
	acquire_spinlock(&raheeq_lock);
	bool lock_already_held = holding_spinlock(&MemFrameLists.mfllock);

				if (!lock_already_held)
				{
					acquire_spinlock(&MemFrameLists.mfllock);
				}

    uint32 VirtualAddress= (uint32) virtual_address;


    if(VirtualAddress >= KERNEL_HEAP_START &&  VirtualAddress< hardLimit ) //Free Block
    {
        free_block(virtual_address);
    }

    else if(VirtualAddress>=(hardLimit + PAGE_SIZE) && VirtualAddress<KERNEL_HEAP_MAX) //Free Pages
    {
    	help++;
        VirtualAddress= ROUNDDOWN(VirtualAddress , PAGE_SIZE); //Aligns the given virtual_address to the nearest lower page boundary.

        uint32 pagenum=(VirtualAddress-KERNEL_HEAP_START)/PAGE_SIZE;



        uint32 found_virtual_address;
        uint32 found_numofpages;

        found_virtual_address=VirtualAddress;
        found_numofpages=Raheeq[pagenum];

        // Unmap the pages
         for (int i = 0; i < found_numofpages; i++)
         {
             unmap_frame(ptr_page_directory, found_virtual_address);
             found_virtual_address += PAGE_SIZE;
         }
        Raheeq[pagenum]=0;
    }
    if (!lock_already_held)
    			{
    				release_spinlock(&MemFrameLists.mfllock);
    			}
    release_spinlock(&raheeq_lock);

}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT'24.MS2 - #05] [1] KERNEL HEAP - kheap_physical_address
	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================
	uint32 *ptr_table ;
	uint32 find=get_page_table(ptr_page_directory, virtual_address, &ptr_table);

	if(find==TABLE_NOT_EXIST)return 0;
	uint32 index_page_table = PTX(virtual_address);

	uint32 page_table_entry = (ptr_table)[index_page_table];
	if(page_table_entry==0)return 0;
	unsigned int offset=virtual_address& 0xFFF;
	unsigned int physical_address = ((page_table_entry >> 12)<< 12) + offset;
	return physical_address;
}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT'24.MS2 - #06] [1] KERNEL HEAP - kheap_virtual_address
	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================
	uint32 page_num=to_frame_info(physical_address)->page_number;
	unsigned int offset=(physical_address&0xFFF);
	return ((page_num)?(page_num+offset):0);

}
//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

/*void *krealloc(void *virtual_address, uint32 new_size) {
    // Case 1: Allocate new memory if virtual_address is NULL
    if (virtual_address == NULL) {
        return kmalloc(new_size);
    }

    // Case 2: Free memory if new_size is 0
    if (new_size == 0) {
        kfree(virtual_address);
        return NULL;
    }

    uint32 VA = (uint32)virtual_address;
    // Ensure the virtual address is valid
    if (VA < KERNEL_HEAP_START || VA >= KERNEL_HEAP_MAX) {
        return NULL;
    }

    uint32 aligned_va = ROUNDDOWN(VA, PAGE_SIZE);
    uint32 page_index = (aligned_va - KERNEL_HEAP_START) / PAGE_SIZE;
    uint32 old_pages = Raheeq[page_index];
    uint32 new_pages = ROUNDUP(new_size, PAGE_SIZE) / PAGE_SIZE;

    // Case 3: Reallocate to the same size (no reallocation)
    if (new_pages == old_pages) {
        return virtual_address;
    }

    // Case 4: Reallocate to smaller size (free unused pages)
    if (new_pages < old_pages) {
        uint32 excess_pages = old_pages - new_pages;
        uint32 free_va_start = aligned_va + new_pages * PAGE_SIZE;

        // Free the excess pages
        for (uint32 i = 0; i < excess_pages; i++) {
            uint32 page_va = free_va_start + i * PAGE_SIZE;
            // Optionally unmap or free frames for excess pages
            // unmap_frame(ptr_page_directory, page_va);
        }

        Raheeq[page_index] = new_pages;
        return virtual_address;
    }

    // Case 5: Reallocate to larger size within contiguous space
    if (new_pages > old_pages) {
        uint32 required_pages = new_pages - old_pages;
        uint32 next_page_index = page_index + old_pages;
        // Check if enough contiguous pages are available
        uint32 contiguous_free = 0;
        for (uint32 i = next_page_index; i < NUM_OF_KHEAP_PAGES; i++) {
            if (Raheeq[i] != 0) break;  // Stop if we encounter a used page
            contiguous_free++;
            if (contiguous_free == required_pages) break;
        }

        // If sufficient contiguous space is available, expand in place
        if (contiguous_free == required_pages) {
            // Allocate the additional pages in the contiguous space
            for (uint32 i = 0; i < required_pages; i++) {
                struct FrameInfo *frame;
                if (allocate_frame(&frame) != E_NO_MEM) {
                    map_frame(ptr_page_directory, frame,
                              (aligned_va + (old_pages + i) * PAGE_SIZE),
                              PERM_PRESENT | PERM_WRITEABLE);
                } else {
                    return NULL; // Allocation failure, not enough frames
                }
            }

            Raheeq[page_index] = new_pages;
            cprintf("rell%d\n",virtual_address);
            return virtual_address;
        }
    }

    // Case 6: Reallocate to larger size requiring relocation
    // Free the old allocation
    //kfree(virtual_address);
    void *new_alloc = kmalloc(new_size);
    if (new_alloc == NULL) {
        return NULL; // Allocation failure
    }




    return new_alloc;
}*/


