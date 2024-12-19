#ifndef FOS_INC_UHEAP_H
#define FOS_INC_UHEAP_H 1

//Values for user heap placement strategy
#define UHP_PLACE_FIRSTFIT 	0x1
#define UHP_PLACE_BESTFIT 	0x2
#define UHP_PLACE_NEXTFIT 	0x3
#define UHP_PLACE_WORSTFIT 	0x4

//2020
#define UHP_USE_BUDDY 0

void *malloc(uint32 size);
void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable);
void* sget(int32 ownerEnvID, char *sharedVarName);
void free(void* virtual_address);
void sfree(void* virtual_address);
void *realloc(void *virtual_address, uint32 new_size);
<<<<<<< HEAD
/*************************************************************************
*************************my update MS2 user heap**************************
**************************************************************************/
void heap_parameters();
int page_allocation;
int num_heap_Pages;
int idx;
=======

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
#endif
