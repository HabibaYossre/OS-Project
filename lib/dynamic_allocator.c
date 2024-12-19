/*
 * dynamic_allocator.c
 *
 *  Created on: Sep 21, 2023
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"

<<<<<<< HEAD
=======

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//=====================================================
// 1) GET BLOCK SIZE (including size of its meta data):
//=====================================================
__inline__ uint32 get_block_size(void* va)
{
	uint32 *curBlkMetaData = ((uint32 *)va - 1) ;
	return (*curBlkMetaData) & ~(0x1);
}

//===========================
// 2) GET BLOCK STATUS:
//===========================
__inline__ int8 is_free_block(void* va)
{
	uint32 *curBlkMetaData = ((uint32 *)va - 1) ;
	return (~(*curBlkMetaData) & 0x1) ;
}

//===========================
// 3) ALLOCATE BLOCK:
//===========================

void *alloc_block(uint32 size, int ALLOC_STRATEGY)
{
	void *va = NULL;
	switch (ALLOC_STRATEGY)
	{
	case DA_FF:
		va = alloc_block_FF(size);
		break;
	case DA_NF:
		va = alloc_block_NF(size);
		break;
	case DA_BF:
		va = alloc_block_BF(size);
		break;
	case DA_WF:
		va = alloc_block_WF(size);
		break;
	default:
		cprintf("Invalid allocation strategy\n");
		break;
	}
	return va;
}

//===========================
// 4) PRINT BLOCKS LIST:
//===========================

void print_blocks_list(struct MemBlock_LIST list)
{
	cprintf("=========================================\n");
	struct BlockElement* blk ;
	cprintf("\nDynAlloc Blocks List:\n");
	LIST_FOREACH(blk, &list)
	{
		cprintf("(size: %d, isFree: %d)\n", get_block_size(blk), is_free_block(blk)) ;
	}
<<<<<<< HEAD

=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	cprintf("=========================================\n");

}
//
////********************************************************************************//
////********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

bool is_initialized = 0;
//==================================
// [1] INITIALIZE DYNAMIC ALLOCATOR:
//==================================
void initialize_dynamic_allocator(uint32 daStart, uint32 initSizeOfAllocatedSpace)
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		if (initSizeOfAllocatedSpace % 2 != 0) initSizeOfAllocatedSpace++; //ensure it's multiple of 2
		if (initSizeOfAllocatedSpace == 0)
			return ;
		is_initialized = 1;
	}
	//==================================================================================
	//==================================================================================

	//TODO: [PROJECT'24.MS1 - #04] [3] DYNAMIC ALLOCATOR - initialize_dynamic_allocator
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
<<<<<<< HEAD
	//panic("initialize_dynamic_allocator is not implemented yet");
	//Your Code is Here...
	LIST_INIT(&freeBlocksList);
				uint32* PokeMaster  = (uint32*)daStart;
				*PokeMaster  = 1;
				PokeMaster  = (uint32*)(daStart + initSizeOfAllocatedSpace - sizeof(uint32));
				*PokeMaster  = 1;
				uint32 size = initSizeOfAllocatedSpace - 2 * sizeof(uint32);
				PokeMaster  = (uint32*)(daStart + sizeof(uint32));
				*PokeMaster  = size;
				PokeMaster  = (uint32*)(daStart + initSizeOfAllocatedSpace - 2 * sizeof(uint32));
				*PokeMaster  = size;
				struct BlockElement* FirstClass = (struct BlockElement*)(daStart + 2 * sizeof(uint32));
							LIST_INSERT_HEAD(&freeBlocksList, FirstClass);
							//cprintf("AAAAAAAAAAAAAAAAinitSizeOfAllocatedSpace: %u\n", initSizeOfAllocatedSpace);

=======
	panic("initialize_dynamic_allocator is not implemented yet");
	//Your Code is Here...
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

}
//==================================
// [2] SET BLOCK HEADER & FOOTER:
//==================================
void set_block_data(void* va, uint32 totalSize, bool isAllocated)
{
<<<<<<< HEAD

		//TODO: [PROJECT'24.MS1 - #05] [3] DYNAMIC ALLOCATOR - set_block_data
		//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//	panic("set_block_data is not implemented yet");
		//Your Code is Here...

			    uint32* header = (uint32*)va - 1;
			    uint32* footer = (uint32*)((char *)header + totalSize - sizeof(uint32)); // should it b *2?

			    *header = totalSize | (isAllocated ? 1 : 0);
			    *footer = totalSize | (isAllocated ? 1 : 0);

=======
	//TODO: [PROJECT'24.MS1 - #05] [3] DYNAMIC ALLOCATOR - set_block_data
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	panic("set_block_data is not implemented yet");
	//Your Code is Here...
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}


//=========================================
// [3] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
<<<<<<< HEAD
void* merg_next(void* va){
	uint32* va_next=(uint32*)(LIST_NEXT((struct BlockElement*)((uint32*)va)));
	uint32* va_cur=(uint32*)va;
	uint32 cur_size=get_block_size((void*)(va_cur));
	uint32 next_size=get_block_size((void*)(va_next));
	set_block_data((void*)(va_cur),cur_size+next_size,0);
	//set_block_data((void*)(va_next),0,0);
	//LIST_INSERT_BEFORE(&freeBlocksList,(struct BlockElement*)va_next,(struct BlockElement*)va_cur);
	LIST_REMOVE(&freeBlocksList,(struct BlockElement*)va_next);
	return (void*)va_cur;
}
void* merg_prev(void* va){
	uint32* va_prev=(uint32*)(LIST_PREV((struct BlockElement*)((uint32*)va)));
	uint32* va_cur=(uint32*)va;
	uint32 cur_size=get_block_size((void*)(va_cur));
	uint32 prev_size=get_block_size((void*)(va_prev));
	set_block_data((void*)(va_prev),cur_size+prev_size,0);
	//set_block_data((void*)(va_cur),0,0);
	LIST_REMOVE(&freeBlocksList,(struct BlockElement*)va_cur);
	return (void*)va_prev;
}
void* alloc_at(void* va,uint32 total_size){
	struct BlockElement* blk=(struct BlockElement*)va;
	uint32 blk_size=get_block_size(blk);
	uint32 leftover_size = blk_size - total_size;
	if(leftover_size>=16){
		set_block_data(blk, total_size, 1);
		void *leftover_ptr = (char *)blk + total_size;
		set_block_data(leftover_ptr, leftover_size, 0);
		LIST_INSERT_AFTER(&freeBlocksList,blk,(struct BlockElement*)leftover_ptr);
	}
	else{
		set_block_data(blk,blk_size,1);
	}
	LIST_REMOVE(&freeBlocksList,blk);

    return (void *)((uint32 *)blk);


}
void *alloc_block_FF(uint32 size)

{
if(size==0)return NULL;
//==================================================================================
//DON'T CHANGE THESE LINES==========================================================
//==================================================================================
{
if (size % 2 != 0) size++;//ensure that the size is even (to use LSB as allocation flag)
if (size < DYN_ALLOC_MIN_BLOCK_SIZE)
size = DYN_ALLOC_MIN_BLOCK_SIZE ;
if (!is_initialized)
{
uint32 required_size = size + 2*sizeof(int) /*header & footer*/ + 2*sizeof(int) /*da begin & end*/ ;
uint32 da_start = (uint32)sbrk(ROUNDUP(required_size, PAGE_SIZE)/PAGE_SIZE);
uint32 da_break = (uint32)sbrk(0);
initialize_dynamic_allocator(da_start, da_break - da_start);

}
}
//==================================================================================
//==================================================================================

//TODO: [PROJECT'24.MS1 - #06] [3] DYNAMIC ALLOCATOR - alloc_block_FF
//COMMENT THE FOLLOWING LINE BEFORE START CODING
//panic("alloc_block_FF is not implemented yet");
//Your Code is Here...


uint32 total_size = size + 2 * sizeof(uint32);
//cprintf("Total size calculated: %u\n", total_size);

struct BlockElement *blk;
LIST_FOREACH(blk, &freeBlocksList) {
    uint32 blk_size = get_block_size(blk);
//    cprintf("Checking block: %p, size: %u\n", blk, blk_size);

    if (blk_size >= total_size) {
        // check splitting
        uint32 leftover_size = blk_size - total_size;
//        cprintf("Block size is sufficient. Leftover size after allocation: %u\n", leftover_size);
                return alloc_at(blk,total_size);
    }
}

    uint32 no_pages = ROUNDUP(total_size, PAGE_SIZE)/PAGE_SIZE; //update here  Calculate number of pages needed
    void *new_space = sbrk(no_pages);

    if (new_space == (void *)-1) {
        return NULL; // sbrk failed, return NULL
    }

    // added
    // Update END block correctly
    int* end_block = (int*)((char*)sbrk(0) - sizeof(int));
    *end_block = 0x1; // Mark the END block


    // Calculate the new block's size
    uint32* up_bound = sbrk(0);
    uint32 cur_size = (uint32)up_bound - (uint32)new_space;

    // Create a new block element

    struct BlockElement* new_alloc_block = (struct BlockElement*)((char*)new_space);//update here
    set_block_data(new_alloc_block, cur_size, 1); // Set block size and allocated flag
    free_block(new_alloc_block); // Add to free list

    // Recursively try to allocate from the free list again
    return alloc_block_FF(size);
//cprintf("New space allocated: %p\n", new_space);
//return (void *)((uint32 *)new_space);
=======
void *alloc_block_FF(uint32 size)
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		if (size % 2 != 0) size++;	//ensure that the size is even (to use LSB as allocation flag)
		if (size < DYN_ALLOC_MIN_BLOCK_SIZE)
			size = DYN_ALLOC_MIN_BLOCK_SIZE ;
		if (!is_initialized)
		{
			uint32 required_size = size + 2*sizeof(int) /*header & footer*/ + 2*sizeof(int) /*da begin & end*/ ;
			uint32 da_start = (uint32)sbrk(ROUNDUP(required_size, PAGE_SIZE)/PAGE_SIZE);
			uint32 da_break = (uint32)sbrk(0);
			initialize_dynamic_allocator(da_start, da_break - da_start);
		}
	}
	//==================================================================================
	//==================================================================================

	//TODO: [PROJECT'24.MS1 - #06] [3] DYNAMIC ALLOCATOR - alloc_block_FF
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	panic("alloc_block_FF is not implemented yet");
	//Your Code is Here...
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

}
//=========================================
// [4] ALLOCATE BLOCK BY BEST FIT:
//=========================================
void *alloc_block_BF(uint32 size)
{
<<<<<<< HEAD
	 if (size == 0) return NULL;
	    size = (size % 2 != 0) ? size + 1 : size;
	    if (size < DYN_ALLOC_MIN_BLOCK_SIZE) size = DYN_ALLOC_MIN_BLOCK_SIZE;

	    uint32 total_size = size + 2 * sizeof(uint32);
	    struct BlockElement *best_fit = NULL;
	    uint32 min_diff = 0xFFFFFFFF;

	    struct BlockElement *blk;
	    LIST_FOREACH(blk, &freeBlocksList) {
	        uint32 blk_size = get_block_size(blk);
	        if (blk_size >= total_size) {
	            uint32 diff = blk_size - total_size;
	            if (diff < min_diff) {
	                min_diff = diff;
	                best_fit = blk;
	            }
	        }
	    }

	    if (best_fit == NULL) {
	    	uint32 no_pages=ROUNDUP(total_size,PAGE_SIZE);
	    		void *new_space = sbrk(no_pages);
	    //		cprintf("Requested new space from sbrk: %p\n", new_space);

	    		if (new_space == (void *)-1) {
	    //		    cprintf("sbrk failed, returning NULL.\n");
	    		    return NULL;
	    		}
	    		uint32* up_bound=sbrk(0);
	    		uint32 new_size=(uint32)up_bound-(uint32)new_space;
	    		struct BlockElement* new_alloc_block=(struct BlockElement*)((char*)new_space+sizeof(uint32));
	    		set_block_data(new_alloc_block,new_size,1);
	    		free_block(new_alloc_block);
	    		return alloc_block_BF(size);
	    }

	    return alloc_at(best_fit, total_size);
=======
	//TODO: [PROJECT'24.MS1 - BONUS] [3] DYNAMIC ALLOCATOR - alloc_block_BF
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	panic("alloc_block_BF is not implemented yet");
	//Your Code is Here...

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}

//===================================================
// [5] FREE BLOCK WITH COALESCING:
//===================================================
<<<<<<< HEAD
/*void* merg_next(void* va){
	uint32* va_next=(uint32*)(LIST_NEXT((struct BlockElement*)((uint32*)va)));
	uint32* va_cur=(uint32*)va;
	uint32 cur_size=get_block_size((void*)(va_cur));
	uint32 next_size=get_block_size((void*)(va_next));
	set_block_data((void*)(va_cur),cur_size+next_size,0);
	//set_block_data((void*)(va_next),0,0);
	//LIST_INSERT_BEFORE(&freeBlocksList,(struct BlockElement*)va_next,(struct BlockElement*)va_cur);
	LIST_REMOVE(&freeBlocksList,(struct BlockElement*)va_next);
	return (void*)va_cur;
}
void* merg_prev(void* va){
	uint32* va_prev=(uint32*)(LIST_PREV((struct BlockElement*)((uint32*)va)));
	uint32* va_cur=(uint32*)va;
	uint32 cur_size=get_block_size((void*)(va_cur));
	uint32 prev_size=get_block_size((void*)(va_prev));
	set_block_data((void*)(va_prev),cur_size+prev_size,0);
	//set_block_data((void*)(va_cur),0,0);
	LIST_REMOVE(&freeBlocksList,(struct BlockElement*)va_cur);
	return (void*)va_prev;
}*/
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
void free_block(void *va)
{
	//TODO: [PROJECT'24.MS1 - #07] [3] DYNAMIC ALLOCATOR - free_block
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
<<<<<<< HEAD
	//panic("free_block is not implemented yet");
	//Your Code is Here...
	if(is_free_block(va)||get_block_size(va)==(uint32)0||va==NULL){
		return;
	}
	//uint32 block_size=get_block_size(va);
	struct BlockElement* Next_block;
	LIST_FOREACH(Next_block,&freeBlocksList){
		if(Next_block>(struct BlockElement*)va){
			break;
		}
	}
	if(Next_block==NULL){
		LIST_INSERT_TAIL(&freeBlocksList,(struct BlockElement*)va);
	}
	else{
	LIST_INSERT_BEFORE(&freeBlocksList,Next_block,(struct BlockElement*)va);
	}
	uint32 block_size=get_block_size(va);
	set_block_data(va,block_size,0);
	struct BlockElement* Prev_block=LIST_PREV((struct BlockElement*)va);
	//uint32 sizeOfNext=get_block_size(Next_block);
	//uint32 sizeOfPrev=get_block_size(Prev_block);

	if(Prev_block&&(uint32*)(get_block_size(Prev_block)+(char*)Prev_block)==(uint32*)va){
		va=merg_prev((void*)va);

	}
	block_size=get_block_size(va);
	struct BlockElement* prt=(struct BlockElement*)((char*)va+block_size);
	if(Next_block&&prt==Next_block){
		va=merg_next((void*)va);

	}



}
//=========================================
// [6] REALLOCATE BLOCK BY FIRST FIT:
//=========================================


void* realloc_block_FF(void* va, uint32 new_size) {

    if (!va) {
        return alloc_block_FF(new_size);
    }


    if (new_size == 0) {
        free_block(va);
        return NULL;
    }

    struct BlockElement* cur_va = (struct BlockElement*)va;
    new_size += 2 * sizeof(uint32);
    uint32 block_size = get_block_size(cur_va);

    if (block_size >= new_size) {

        if (block_size - new_size >= 16) {
            set_block_data(cur_va, new_size, 1);
            struct BlockElement* leftover_block = (struct BlockElement*)((char*)cur_va + new_size);
            set_block_data(leftover_block, block_size - new_size, 1);
            free_block(leftover_block);
        } else {
            set_block_data(cur_va, block_size, 1);
        }
        return va;
    }


    struct BlockElement* next_block = (struct BlockElement*)((char*)cur_va + block_size);


    if (is_free_block(next_block) && (block_size + get_block_size(next_block) >= new_size)) {
        uint32 total_size = block_size + get_block_size(next_block);

        if (total_size - new_size >= 16) {
            set_block_data(cur_va, new_size, 1);
            struct BlockElement* leftover_block = (struct BlockElement*)((char*)cur_va + new_size);
            set_block_data(leftover_block, total_size - new_size, 1);
            free_block(leftover_block);
        } else {
            set_block_data(cur_va, total_size, 1);
        }

        LIST_REMOVE(&freeBlocksList, next_block);
        return va;
    }
    void* ans = alloc_block_FF(new_size - 2 * sizeof(uint32));
    if (ans) {
        uint32 copy_size = block_size - 2 * sizeof(uint32);
        if (copy_size > new_size) {
            copy_size = new_size;
        }
        memcpy(ans, va, copy_size);
        free_block(va);
    } else {

        return va;
    }

    return ans;
}


=======
	panic("free_block is not implemented yet");
	//Your Code is Here...
}

//=========================================
// [6] REALLOCATE BLOCK BY FIRST FIT:
//=========================================
void *realloc_block_FF(void* va, uint32 new_size)
{
	//TODO: [PROJECT'24.MS1 - #08] [3] DYNAMIC ALLOCATOR - realloc_block_FF
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	panic("realloc_block_FF is not implemented yet");
	//Your Code is Here...
}

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
//=========================================
// [7] ALLOCATE BLOCK BY WORST FIT:
//=========================================
void *alloc_block_WF(uint32 size)
{
	panic("alloc_block_WF is not implemented yet");
	return NULL;
}

//=========================================
// [8] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
void *alloc_block_NF(uint32 size)
{
	panic("alloc_block_NF is not implemented yet");
	return NULL;
}
