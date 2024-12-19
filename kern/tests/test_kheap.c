<<<<<<< HEAD
#include <kern/tests/test_kheap.h>
=======
#include "test_kheap.h"
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

#include <inc/memlayout.h>
#include <inc/queue.h>
#include <inc/dynamic_allocator.h>
#include <kern/cpu/sched.h>
#include <kern/disk/pagefile_manager.h>
#include "../mem/kheap.h"
#include "../mem/memory_manager.h"


#define Mega  (1024*1024)
#define kilo (1024)

//2017
#define DYNAMIC_ALLOCATOR_DS 0 //ROUNDUP(NUM_OF_KHEAP_PAGES * sizeof(struct MemBlock), PAGE_SIZE)
<<<<<<< HEAD
#define INITIAL_KHEAP_ALLOCATIONS (DYNAMIC_ALLOCATOR_DS) //( + KERNEL_SHARES_ARR_INIT_SIZE + KERNEL_SEMAPHORES_ARR_INIT_SIZE) //
#define INITIAL_BLOCK_ALLOCATIONS ((2*sizeof(int) + MAX(num_of_ready_queues * sizeof(uint8), DYN_ALLOC_MIN_BLOCK_SIZE)) + (2*sizeof(int) + MAX(num_of_ready_queues * sizeof(struct Env_Queue), DYN_ALLOC_MIN_BLOCK_SIZE)))
=======
#define INITIAL_KHEAP_ALLOCATIONS (DYNAMIC_ALLOCATOR_DS) //( + KERNEL_SHARES_ARR_INIT_SIZE + KERNEL_SEMAPHORES_ARR_INIT_SIZE) // + ROUNDUP(num_of_ready_queues * sizeof(uint8), PAGE_SIZE) + ROUNDUP(num_of_ready_queues * sizeof(struct Env_Queue), PAGE_SIZE))
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
#define ACTUAL_START ((KERNEL_HEAP_START + DYN_ALLOC_MAX_SIZE + PAGE_SIZE) + INITIAL_KHEAP_ALLOCATIONS)

extern uint32 sys_calculate_free_frames() ;
extern void sys_bypassPageFault(uint8);
extern uint32 sys_rcr2();
extern int execute_command(char *command_string);

extern char end_of_kernel[];

extern int CB(uint32 *ptr_dir, uint32 va, int bn);

struct MyStruct
{
	char a;
	short b;
	int c;
};

uint32 da_limit = KERNEL_HEAP_START + DYN_ALLOC_MAX_SIZE ;
int test_kmalloc()
{
	/*********************** NOTE ****************************
	 * WE COMPARE THE DIFF IN FREE FRAMES BY "AT LEAST" RULE
	 * INSTEAD OF "EQUAL" RULE SINCE IT'S POSSIBLE FOR SOME
	 * IMPLEMENTATIONS TO DYNAMICALLY ALLOCATE SPECIAL DATA
	 * STRUCTURE TO MANAGE THE PAGE ALLOCATOR.
	 *********************************************************/

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	char *byteArr, *byteArr2, *byteArr3 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfByte3, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = (int)sys_calculate_free_frames() ;
	int eval = 0;
	bool correct = 1 ;
	int freeFrames, freeDiskFrames;
	uint32 sizeOfKHeap;
	void* ptr_allocations[20] = {0};
<<<<<<< HEAD

	cprintf("\n1. Insufficient space [10%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	correct = 1 ;
	{
		//Insufficient space
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		sizeOfKHeap = (KERNEL_HEAP_MAX - ACTUAL_START + 1) ;
		ptr_allocations[0] = kmalloc(sizeOfKHeap);
		if (ptr_allocations[0] != NULL) { correct = 0; cprintf("Allocating insufficient space: should return NULL\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
	}
	if (correct)	eval+=10 ;

<<<<<<< HEAD
	cprintf("\n2. Allocate spaces of different sizes in BOTH ALLOCATORS [40%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	correct = 1 ;
	{
		//2 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[0] !=  (ACTUAL_START)) { correct = 0; cprintf("1 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("1 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[0] !=  (ACTUAL_START)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//2 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("2 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("2 Wrong allocation: pages are not loaded successfully into memory\n"); }

		freeFrames = (int)sys_calculate_free_frames() ;
		{
			//2 KB - 1 (should be allocated by dynamic allocator not page allocator)
			freeDiskFrames = (int)pf_calculate_free_frames() ;
			ptr_allocations[2] = kmalloc(2*kilo-1);
			if ((uint32) ptr_allocations[2] < KERNEL_HEAP_START || ptr_allocations[2] >= sbrk(0) || (uint32) ptr_allocations[2] >= da_limit)
			{ correct = 0; cprintf("3 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			//if ((freeFrames - (int)sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

			//2 KB - 1 (should be allocated by dynamic allocator not page allocator)
			freeDiskFrames = (int)pf_calculate_free_frames() ;
			ptr_allocations[3] = kmalloc(2*kilo-1);
			if ((uint32) ptr_allocations[3] < KERNEL_HEAP_START || ptr_allocations[3] >= sbrk(0) || (uint32) ptr_allocations[3] >= da_limit)
			{ correct = 0; cprintf("4 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("4 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		}
		if ((freeFrames - (int)sys_calculate_free_frames()) != 1) { correct = 0; cprintf("4 Wrong allocation: sbrk error\n"); }
=======
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

		//2 KB - 1 (should be allocated by dynamic allocator not page allocator)
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[2] = kmalloc(2*kilo-1);
		if ((uint32) ptr_allocations[2] < KERNEL_HEAP_START || ptr_allocations[2] >= sbrk(0) || (uint32) ptr_allocations[2] >= da_limit)
		{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		//if ((freeFrames - (int)sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

		//2 KB - 1 (should be allocated by dynamic allocator not page allocator)
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[3] = kmalloc(2*kilo-1);
		if ((uint32) ptr_allocations[3] < KERNEL_HEAP_START || ptr_allocations[3] >= sbrk(0) || (uint32) ptr_allocations[3] >= da_limit)
		{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		//if ((freeFrames - (int)sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//7 KB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[4] = kmalloc(7*kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[4] != (ACTUAL_START + 4*Mega /*+ 8*kilo*/)) { correct = 0; cprintf("5 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("5 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 2) { correct = 0; cprintf("5 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[4] != (ACTUAL_START + 4*Mega /*+ 8*kilo*/)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 2) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//3 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(3*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega + 8*kilo) ) { correct = 0; cprintf("6 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("6 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 768) { correct = 0; cprintf("6 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega + 8*kilo) ) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 768) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//6 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(6*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("7 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("7 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 1536) { correct = 0; cprintf("7 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 1536) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//14 KB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(14*kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 13*Mega + 8*kilo)) { correct = 0; cprintf("8 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("8 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 4) { correct = 0; cprintf("8 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 13*Mega + 8*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 4) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=40 ;

	correct = 1 ;
	//Checking read/write on the allocated spaces
<<<<<<< HEAD
	cprintf("\n3. Checking read/write on the allocated spaces [30%]\n");
	{
=======
	{

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;

		//Write values
		//In 1st 2 MB
		lastIndexOfByte = (2*Mega-kilo)/sizeof(char) - 1;
		byteArr = (char *) ptr_allocations[0];
		byteArr[0] = minByte ;
		byteArr[lastIndexOfByte] = maxByte ;

		//In 2nd 2 MB
		shortArr = (short *) ptr_allocations[1];
		lastIndexOfShort = (2*Mega-kilo)/sizeof(short) - 1;
		shortArr[0] = minShort;
		shortArr[lastIndexOfShort] = maxShort;

		//In Dynamic Allocator Area
		{
			//In 2 KB - 1
			intArr = (int *) ptr_allocations[2];
			lastIndexOfInt = (2*kilo-1)/sizeof(int) - 1;
			intArr[0] = minInt;
			intArr[lastIndexOfInt] = maxInt;

			//In 2 KB - 1
			byteArr2 = (char *) ptr_allocations[3];
			lastIndexOfByte2 = (2*kilo-1)/sizeof(char) - 1;
			byteArr2[0] = minByte;
			byteArr2[lastIndexOfByte2] = maxByte;
		}

		//In 7 KB
		structArr = (struct MyStruct *) ptr_allocations[4];
		lastIndexOfStruct = (7*kilo)/sizeof(struct MyStruct) - 1;
		structArr[0].a = minByte; structArr[0].b = minShort; structArr[0].c = minInt;
		structArr[lastIndexOfStruct].a = maxByte; structArr[lastIndexOfStruct].b = maxShort; structArr[lastIndexOfStruct].c = maxInt;

		//In 6 MB
		lastIndexOfByte3 = (6*Mega-kilo)/sizeof(char) - 1;
		byteArr3 = (char *) ptr_allocations[6];
		byteArr3[0] = minByte ;
		byteArr3[lastIndexOfByte3 / 2] = maxByte / 2;
		byteArr3[lastIndexOfByte3] = maxByte ;

		//In 14 KB
		shortArr2 = (short *) ptr_allocations[7];
		lastIndexOfShort2 = (14*kilo)/sizeof(short) - 1;
		shortArr2[0] = minShort;
		shortArr2[lastIndexOfShort2] = maxShort;

		//Read values: check that the values are successfully written
<<<<<<< HEAD
		if (byteArr[0] 	!= minByte 	|| byteArr[lastIndexOfByte] 	!= maxByte) { correct = 0; cprintf("9 Wrong allocation: stored values are wrongly changed!\n"); }
		if (shortArr[0] != minShort || shortArr[lastIndexOfShort] 	!= maxShort) { correct = 0; cprintf("10 Wrong allocation: stored values are wrongly changed!\n"); }
		if (intArr[0] 	!= minInt 	|| intArr[lastIndexOfInt] 		!= maxInt) { correct = 0; cprintf("11 Wrong allocation: stored values are wrongly changed!\n"); }
		if (byteArr2[0] != minByte || byteArr2[lastIndexOfByte2] != maxByte) { correct = 0; cprintf("12 Wrong allocation: stored values are wrongly changed!\n"); }

		if (structArr[0].a != minByte 	|| structArr[lastIndexOfStruct].a != maxByte) 	{ correct = 0; cprintf("13 Wrong allocation: stored values are wrongly changed!\n"); }
		if (structArr[0].b != minShort 	|| structArr[lastIndexOfStruct].b != maxShort) 	{ correct = 0; cprintf("14 Wrong allocation: stored values are wrongly changed!\n"); }
		if (structArr[0].c != minInt 	|| structArr[lastIndexOfStruct].c != maxInt) 	{ correct = 0; cprintf("15 Wrong allocation: stored values are wrongly changed!\n"); }

		if (byteArr3[0] != minByte || byteArr3[lastIndexOfByte3/2] != maxByte/2 || byteArr3[lastIndexOfByte3] != maxByte) { correct = 0; cprintf("Wrong allocation: stored values are wrongly changed!\n"); }
		if (shortArr2[0] != minShort || shortArr2[lastIndexOfShort2] != maxShort) { correct = 0; cprintf("16 Wrong allocation: stored values are wrongly changed!\n"); }

		if ((freeFrames - (int)sys_calculate_free_frames()) != 0) { correct = 0; cprintf("17 Wrong allocation: pages are not loaded successfully into memory\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("17 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
=======
		if (byteArr[0] 	!= minByte 	|| byteArr[lastIndexOfByte] 	!= maxByte) { correct = 0; cprintf("Wrong allocation: stored values are wrongly changed!\n"); }
		if (shortArr[0] != minShort || shortArr[lastIndexOfShort] 	!= maxShort) { correct = 0; cprintf("Wrong allocation: stored values are wrongly changed!\n"); }
		if (intArr[0] 	!= minInt 	|| intArr[lastIndexOfInt] 		!= maxInt) { correct = 0; cprintf("Wrong allocation: stored values are wrongly changed!\n"); }
		if (byteArr2[0] != minByte || byteArr2[lastIndexOfByte2] != maxByte) { correct = 0; cprintf("Wrong allocation: stored values are wrongly changed!\n"); }

		if (structArr[0].a != minByte 	|| structArr[lastIndexOfStruct].a != maxByte) 	{ correct = 0; cprintf("Wrong allocation: stored values are wrongly changed!\n"); }
		if (structArr[0].b != minShort 	|| structArr[lastIndexOfStruct].b != maxShort) 	{ correct = 0; cprintf("Wrong allocation: stored values are wrongly changed!\n"); }
		if (structArr[0].c != minInt 	|| structArr[lastIndexOfStruct].c != maxInt) 	{ correct = 0; cprintf("Wrong allocation: stored values are wrongly changed!\n"); }

		if (byteArr3[0] != minByte || byteArr3[lastIndexOfByte3/2] != maxByte/2 || byteArr3[lastIndexOfByte3] != maxByte) { correct = 0; cprintf("Wrong allocation: stored values are wrongly changed!\n"); }
		if (shortArr2[0] != minShort || shortArr2[lastIndexOfShort2] != maxShort) { correct = 0; cprintf("Wrong allocation: stored values are wrongly changed!\n"); }

		if ((freeFrames - (int)sys_calculate_free_frames()) != 0) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

	}
	if (correct)	eval+=30 ;

	correct = 1 ;
	//Insufficient space again
<<<<<<< HEAD
	cprintf("\n4. Insufficient Space Again [10%]\n");
	{
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		uint32 restOfKHeap = (KERNEL_HEAP_MAX - ACTUAL_START) - (2*Mega+2*Mega+/*4*kilo+4*kilo+*/8*kilo+3*Mega+6*Mega+16*kilo) ;
		ptr_allocations[8] = kmalloc(restOfKHeap+1);
		if (ptr_allocations[8] != NULL) { correct = 0; cprintf("18 Allocating insufficient space: should return NULL\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("18 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0) { correct = 0; cprintf("18 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
	{
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		uint32 restOfKHeap = (KERNEL_HEAP_MAX - ACTUAL_START + 2*PAGE_SIZE) - (2*Mega+2*Mega+/*4*kilo+4*kilo+*/8*kilo+3*Mega+6*Mega+16*kilo) ;
		ptr_allocations[8] = kmalloc(restOfKHeap);
		if (ptr_allocations[8] != NULL) { correct = 0; cprintf("Allocating insufficient space: should return NULL\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=10 ;

	correct = 1 ;
	//permissions
<<<<<<< HEAD
	cprintf("\n5. Check permissions of allocated spaces in PAGE ALLOCATOR [10%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		uint32 lastAllocAddress = (uint32)ptr_allocations[7] + 16*kilo ;
		uint32 va;
		for (va = ACTUAL_START; va < lastAllocAddress; va+=PAGE_SIZE)
		{
			unsigned int * table;
			get_page_table(ptr_page_directory, va, &table);
			uint32 perm = table[PTX(va)] & 0xFFF;
			if ((perm & PERM_USER) == PERM_USER)
			{
				if (correct)
				{
<<<<<<< HEAD
					correct = 0; cprintf("19 Wrong permissions: pages should be mapped with Supervisor permission only\n");
=======
					correct = 0; cprintf("Wrong permissions: pages should be mapped with Supervisor permission only\n");
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
				}
			}
		}
	}
	if (correct)	eval+=10 ;

	cprintf("\ntest kmalloc completed. Evaluation = %d%\n", eval);

	return 1;

}


int test_kmalloc_firstfit1()
{
	/*********************** NOTE ****************************
	 * WE COMPARE THE DIFF IN FREE FRAMES BY "AT LEAST" RULE
	 * INSTEAD OF "EQUAL" RULE SINCE IT'S POSSIBLE FOR SOME
	 * IMPLEMENTATIONS TO DYNAMICALLY ALLOCATE SPECIAL DATA
	 * STRUCTURE TO MANAGE THE PAGE ALLOCATOR.
	 *********************************************************/

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	void* ptr_allocations[20] = {0};
<<<<<<< HEAD
	int freeFrames;
	int freeDiskFrames;
=======
	uint32 freeFrames;
	uint32 freeDiskFrames;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	int eval = 0;
	bool correct = 1 ;

	correct = 1 ;
	//[1] Allocate all
<<<<<<< HEAD
	cprintf("\n1. Allocate spaces of different sizes in PAGE ALLOCATOR [10%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		//Allocate 1 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(1*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[0] != (ACTUAL_START)) { correct = 0; cprintf("1 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("1 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[0] != (ACTUAL_START)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//Allocate 1 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(1*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 1*Mega)) { correct = 0; cprintf("2 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("2 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 1*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//Allocate 1 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[2] = kmalloc(1*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[2] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("3 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("3 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[2] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//Allocate 1 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[3] = kmalloc(1*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[3] != (ACTUAL_START + 3*Mega)) { correct = 0; cprintf("4 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("4 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("4 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[3] != (ACTUAL_START + 3*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//Allocate 2 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[4] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[4] != (ACTUAL_START + 4*Mega)) { correct = 0; cprintf("5 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("5 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("5 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[4] != (ACTUAL_START + 4*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//Allocate 2 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 6*Mega)) { correct = 0; cprintf("6 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("6 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("6 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 6*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//Allocate 3 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(3*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[6] !=  (ACTUAL_START + 8*Mega)) { correct = 0; cprintf("7 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("7 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 768) { correct = 0; cprintf("7 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[6] !=  (ACTUAL_START + 8*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 768) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//Allocate 3 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(3*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 11*Mega)) { correct = 0; cprintf("8 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("8 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 768) { correct = 0; cprintf("8 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 11*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 768) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=10 ;

	correct = 1 ;
	//[2] Free some to create holes
<<<<<<< HEAD
	cprintf("\n2. Free some to create holes [10%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		//1 MB Hole
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
<<<<<<< HEAD
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("9 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 256) { correct = 0; cprintf("9 Wrong free: \n"); }
=======
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 256) { correct = 0; cprintf("Wrong free: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//2 MB Hole
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		kfree(ptr_allocations[4]);
<<<<<<< HEAD
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("10 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("10 Wrong free: \n"); }
=======
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("Wrong free: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//3 MB Hole
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		kfree(ptr_allocations[6]);
<<<<<<< HEAD
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("11 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 768) { correct = 0; cprintf("11 Wrong free: \n"); }
=======
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 768) { correct = 0; cprintf("Wrong free: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=10 ;

	correct = 1 ;
	//[3] Allocate again [test first fit]
<<<<<<< HEAD
	cprintf("\n3. Allocate again [test first fit] [40%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		//Allocate 512 KB - should be placed in 1st hole
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[8] = kmalloc(512*kilo - kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 1*Mega)) { correct = 0; cprintf("12 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("12 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 128) { correct = 0; cprintf("12 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 1*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 128) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//Allocate 1 MB - should be placed in 2nd hole
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[9] = kmalloc(1*Mega - kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[9] != (ACTUAL_START + 4*Mega)) { correct = 0; cprintf("13 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("13 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("13 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[9] != (ACTUAL_START + 4*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c


		//Allocate 256 KB - should be placed in remaining of 1st hole
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[10] = kmalloc(256*kilo - kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[10] != (ACTUAL_START + 1*Mega + 512*kilo)) { correct = 0; cprintf("14 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("14 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 64) { correct = 0; cprintf("14 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[10] != (ACTUAL_START + 1*Mega + 512*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 64) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//Allocate 2 MB - should be placed in 3rd hole
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[11] = kmalloc(2*Mega);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[11] != (ACTUAL_START + 8*Mega)) { correct = 0; cprintf("15 Wrong start address for the allocated space... \n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("15 Wrong allocation: \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("15 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
=======
		if ((uint32) ptr_allocations[11] != (ACTUAL_START + 8*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c


		//Allocate 4 MB - should be placed in end of all allocations
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[12] = kmalloc(4*Mega - kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[12] != (ACTUAL_START + 14*Mega)) { correct = 0; cprintf("16 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("16 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 1024) { correct = 0; cprintf("16 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[12] != (ACTUAL_START + 14*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 1024) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=40 ;

	correct = 1 ;
	//[4] Free contiguous allocations
<<<<<<< HEAD
	cprintf("\n4. Free contiguous allocations (coalescing) [10%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		//1 MB Hole appended to previous 256 KB hole
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		kfree(ptr_allocations[2]);
<<<<<<< HEAD
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("17 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 256) { correct = 0; cprintf("17 Wrong free: \n"); }
=======
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 256) { correct = 0; cprintf("Wrong free: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//Next 1 MB Hole appended also
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		kfree(ptr_allocations[3]);
<<<<<<< HEAD
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("18 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 256) { correct = 0; cprintf("18 Wrong free: \n"); }
=======
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 256) { correct = 0; cprintf("Wrong free: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=10 ;

	correct = 1 ;
	//[5] Allocate again [test first fit]
<<<<<<< HEAD
	cprintf("\n5. Allocate again [test first fit in coalesced area] [30%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		//[FIRST FIT Case]
		//Allocate 1 MB - should be placed in the contiguous hole (256 KB + 2 MB)
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[13] = kmalloc(1*Mega);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[13] != (ACTUAL_START + 1*Mega + 768*kilo)) { correct = 0; cprintf("19 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("19 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("19 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[13] != (ACTUAL_START + 1*Mega + 768*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 256) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=30 ;

	cprintf("test FIRST FIT allocation (1) completed. Eval = %d%\n", eval);

	return 1;
}

int test_kmalloc_firstfit2()
{
	/*********************** NOTE ****************************
	 * WE COMPARE THE DIFF IN FREE FRAMES BY "AT LEAST" RULE
	 * INSTEAD OF "EQUAL" RULE SINCE IT'S POSSIBLE FOR SOME
	 * IMPLEMENTATIONS TO DYNAMICALLY ALLOCATE SPECIAL DATA
	 * STRUCTURE TO MANAGE THE PAGE ALLOCATOR.
	 *********************************************************/

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	void* ptr_allocations[20] = {0};
<<<<<<< HEAD
	int freeFrames;
	int freeDiskFrames;
=======
	uint32 freeFrames;
	uint32 freeDiskFrames;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	int eval = 0;
	bool correct = 1 ;

	correct = 1 ;
	//[1] Attempt to allocate more than heap size
<<<<<<< HEAD
	cprintf("\n1. Attempt to allocate more than heap size [10%]\n");
	{
		ptr_allocations[0] = kmalloc(KERNEL_HEAP_MAX - ACTUAL_START + 1);
		if (ptr_allocations[0] != NULL) { correct = 0; cprintf("1 kmalloc: Attempt to allocate more than heap size, should return NULL\n"); }
=======
	{
		ptr_allocations[0] = kmalloc(KERNEL_HEAP_MAX - ACTUAL_START + 1);
		if (ptr_allocations[0] != NULL) { correct = 0; cprintf("kmalloc: Attempt to allocate more than heap size, should return NULL\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=10 ;

	correct = 1 ;
	//[2] Attempt to allocate space more than any available fragment
	//	a) Create Fragments
<<<<<<< HEAD
	cprintf("\n2. Allocate & Free to create fragments inside BOTH ALLOCATORS [10%]\n");
	{
		/*[1] PAGE ALLOCATOR PART*/

=======
	{
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		//2 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[0] != (ACTUAL_START)) { correct = 0; cprintf("2 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("2 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[0] != (ACTUAL_START)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//2 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("3 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("3 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: \n"); }

		//1 KB (should be allocated by dynamic allocator not page allocator)
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[2] = kmalloc(1*kilo);
		if ((uint32) ptr_allocations[2] < KERNEL_HEAP_START || ptr_allocations[2] >= sbrk(0) || (uint32) ptr_allocations[2] >= da_limit)
		{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		//if ((freeFrames - (int)sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

		//2 KB (should be allocated by dynamic allocator not page allocator)
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[3] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[3] < KERNEL_HEAP_START || ptr_allocations[3] >= sbrk(0) || (uint32) ptr_allocations[3] >= da_limit)
		{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		//if ((freeFrames - (int)sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

		//1 KB (should be allocated by dynamic allocator not page allocator)
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[4] = kmalloc(1*kilo);
		if ((uint32) ptr_allocations[4] < KERNEL_HEAP_START || ptr_allocations[4] >= sbrk(0) || (uint32) ptr_allocations[4] >= da_limit)
		{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		//if ((freeFrames - (int)sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

		//1 KB Hole in Dynamic Allocator Area
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		kfree(ptr_allocations[2]);
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("Wrong free: freeing a block from the dynamic allocator should not affect the free frames\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//7 KB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(7*kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega /*+ 8*kilo*/)) { correct = 0; cprintf("4 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("4 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 2) { correct = 0; cprintf("4 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega /*+ 8*kilo*/)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 2) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//2 MB Hole
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
<<<<<<< HEAD
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("5 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("5 Wrong free: \n"); }
=======
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("Wrong free: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//3 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(3*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 4*Mega + 8*kilo)) { correct = 0; cprintf("6 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("6 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) <  3*Mega/PAGE_SIZE) { correct = 0; cprintf("6 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 4*Mega + 8*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) <  3*Mega/PAGE_SIZE) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//2 MB + 6 KB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(2*Mega + 6*kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("7 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("7 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) <  514) { correct = 0; cprintf("7 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) <  514) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//3 MB Hole
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		kfree(ptr_allocations[6]);
<<<<<<< HEAD
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("8 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 768) { correct = 0; cprintf("8 Wrong free: \n"); }
=======
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 768) { correct = 0; cprintf("Wrong free: \n"); }

		//2 KB Hole in Dynamic Allocator Area [Resulting Hole = 1 KB + 2 KB = 3 KB]
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		kfree(ptr_allocations[3]);
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("Wrong free: freeing a block from the dynamic allocator should not affect the free frames\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//2 MB Hole [Resulting Hole = 2 MB + 2 MB = 4 MB]
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
<<<<<<< HEAD
		if (((int)sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("9 Wrong free: \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("9 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
=======
		if (((int)sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("Wrong free: \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//5 MB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[8] = kmalloc(5*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 9*Mega + 16*kilo)) { correct = 0; cprintf("10 Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("10 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) <   5*Mega/PAGE_SIZE) { correct = 0; cprintf("10 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 9*Mega + 16*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) <   5*Mega/PAGE_SIZE) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//8 KB Hole [Resulting Hole = 2 MB + 2 MB + 8 KB + 3 MB = 7 MB + 8 KB]
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		kfree(ptr_allocations[5]);
<<<<<<< HEAD
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("11 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 2) { correct = 0; cprintf("11 Wrong free: \n"); }


		/*[2] BLOCK ALLOCATOR PART*/
		freeFrames = (int)sys_calculate_free_frames() ;
		{
			//1 KB (should be allocated by dynamic allocator not page allocator)
			freeDiskFrames = (int)pf_calculate_free_frames() ;
			ptr_allocations[2] = kmalloc(1*kilo);
			if ((uint32) ptr_allocations[2] < KERNEL_HEAP_START || ptr_allocations[2] >= sbrk(0) || (uint32) ptr_allocations[2] >= da_limit)
			{ correct = 0; cprintf("12 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("12 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }

			//2 KB (should be allocated by dynamic allocator not page allocator)
			freeDiskFrames = (int)pf_calculate_free_frames() ;
			ptr_allocations[3] = kmalloc(2*kilo);
			if ((uint32) ptr_allocations[3] < KERNEL_HEAP_START || ptr_allocations[3] >= sbrk(0) || (uint32) ptr_allocations[3] >= da_limit)
			{ correct = 0; cprintf("13 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("13 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }

			//1 KB (should be allocated by dynamic allocator not page allocator)
			freeDiskFrames = (int)pf_calculate_free_frames() ;
			ptr_allocations[4] = kmalloc(1*kilo);
			if ((uint32) ptr_allocations[4] < KERNEL_HEAP_START || ptr_allocations[4] >= sbrk(0) || (uint32) ptr_allocations[4] >= da_limit)
			{ correct = 0; cprintf("14 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("14 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		}
		if ((freeFrames - (int)sys_calculate_free_frames()) != 1) { correct = 0; cprintf("14 Wrong allocation: sbrk error\n"); }

		{
			//1 KB Hole in Dynamic Allocator Area
			freeFrames = (int)sys_calculate_free_frames() ;
			freeDiskFrames = (int)pf_calculate_free_frames() ;
			kfree(ptr_allocations[2]);
			if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("15 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if (((int)sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("15 Wrong free: freeing a block from the dynamic allocator should not affect the free frames\n"); }

			//2 KB Hole in Dynamic Allocator Area [Resulting Hole = 1 KB + 2 KB = 3 KB]
			freeFrames = (int)sys_calculate_free_frames() ;
			freeDiskFrames = (int)pf_calculate_free_frames() ;
			kfree(ptr_allocations[3]);
			if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("16 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if (((int)sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("16 Wrong free: freeing a block from the dynamic allocator should not affect the free frames\n"); }
		}
	}
	if (correct)	eval+=10 ;

	/*[1] FF: BLOCK ALLOCATOR PART*/
	cprintf("\n3. [BLOCK ALLOCATOR] Test First Fit Strategy [35%]\n");
	correct = 1 ;
	{
		//[FIRST FIT Case#3] Should be allocated in the resulting hole inside DYNAMIC Allocator Area
		//2 KB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[11] = kmalloc(2*kilo);
		if (ptr_allocations[11] != ptr_allocations[2])
		{ correct = 0; cprintf("17 Wrong start address for the allocated space... \n"); }
		if((freeDiskFrames - (int)pf_calculate_free_frames()) !=  0)  { correct = 0; cprintf("17 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0) { correct = 0; cprintf("17 Wrong allocation: \n"); }

		//[FIRST FIT Case#4] Should be allocated in the remaining of resulting hole inside DYNAMIC Allocator Area
		//1 KB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[12] = kmalloc(1*kilo);
		if (ptr_allocations[12] != ptr_allocations[2] + 2*kilo + 2*sizeof(int) /*footer & next header*/)
		{ correct = 0; cprintf("18 Wrong start address for the allocated space... \n"); }
		if((freeDiskFrames - (int)pf_calculate_free_frames()) !=  0)  { correct = 0; cprintf("18 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0) { correct = 0; cprintf("18 Wrong allocation: \n"); }

	}
	if (correct)	eval+=35 ;

	/*[2] FF: PAGE ALLOCATOR PART*/
	cprintf("\n4. [PAGE ALLOCATOR] Test First Fit Strategy [35%]\n");
=======
		if(((int)pf_calculate_free_frames() - freeDiskFrames) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if (((int)sys_calculate_free_frames() - freeFrames) < 2) { correct = 0; cprintf("Wrong free: \n"); }
	}
	if (correct)	eval+=10 ;

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	correct = 1 ;
	{
		//[FIRST FIT Case#1] Should be allocated in the resulting hole inside Page Allocator Area
		//7 MB + 1 KB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[9] = kmalloc(7*Mega+kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[9] != (ACTUAL_START)) { correct = 0; cprintf("19 Wrong start address for the allocated space... \n"); }
		if((freeDiskFrames - (int)pf_calculate_free_frames()) !=  0)  { correct = 0; cprintf("19 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) <  (7*Mega+4*kilo)/PAGE_SIZE) { correct = 0; cprintf("19 Wrong allocation: \n"); }
=======
		if ((uint32) ptr_allocations[9] != (ACTUAL_START)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if((freeDiskFrames - (int)pf_calculate_free_frames()) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) <  (7*Mega+4*kilo)/PAGE_SIZE) { correct = 0; cprintf("Wrong allocation: \n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//[FIRST FIT Case#2] Should be allocated in the remaining area of resulting hole inside Page Allocator Area
		//3 KB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[10] = kmalloc(3*kilo);
<<<<<<< HEAD
		if ((uint32)ptr_allocations[10] != (ACTUAL_START + 7*Mega + 4*kilo)) { correct = 0; cprintf("20 Wrong start address for the allocated space... \n"); }
		if((freeDiskFrames - (int)pf_calculate_free_frames()) !=  0)  { correct = 0; cprintf("20 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 1) { correct = 0; cprintf("20 Wrong allocation: \n"); }
	}
	if (correct)	eval+=35 ;


	correct = 1 ;
	//	b) Attempt to allocate large segment with no suitable fragment to fit on
	cprintf("\n5. Attempt to allocate large segment with no suitable fragment to fit on [10%]\n");
	{
		//Large Allocation
		ptr_allocations[13] = kmalloc((KERNEL_HEAP_MAX - ACTUAL_START - 14*Mega));
		if (ptr_allocations[13] != NULL) { correct = 0; cprintf("21 Kmalloc: Attempt to allocate large segment with no suitable fragment to fit on, should return NULL\n"); }
=======
		if ((uint32)ptr_allocations[10] != (ACTUAL_START + 7*Mega + 4*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if((freeDiskFrames - (int)pf_calculate_free_frames()) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) < 1) { correct = 0; cprintf("Wrong allocation: \n"); }
	}
	if (correct)	eval+=35 ;

	correct = 1 ;
	{
		//[FIRST FIT Case#3] Should be allocated in the resulting hole inside DYNAMIC Allocator Area
		//1 KB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[11] = kmalloc(1*kilo);
		if ((ptr_allocations[11] < ptr_allocations[2]) || (ptr_allocations[11] > (ptr_allocations[2] + 1*kilo)))
		{ correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if((freeDiskFrames - (int)pf_calculate_free_frames()) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0) { correct = 0; cprintf("Wrong allocation: \n"); }

		//[FIRST FIT Case#4] Should be allocated in the remaining of resulting hole inside DYNAMIC Allocator Area
		//1 KB
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[12] = kmalloc(1*kilo);
		if ((ptr_allocations[12] < ptr_allocations[2] + 1*kilo) || (ptr_allocations[12] > (ptr_allocations[2] + 2*kilo)))
		{ correct = 0; cprintf("Wrong start address for the allocated space... \n"); }
		if((freeDiskFrames - (int)pf_calculate_free_frames()) !=  0)  { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0) { correct = 0; cprintf("Wrong allocation: \n"); }

	}
	if (correct)	eval+=35 ;

	correct = 1 ;
	//	b) Attempt to allocate large segment with no suitable fragment to fit on
	{
		//Large Allocation
		ptr_allocations[13] = kmalloc((KERNEL_HEAP_MAX - ACTUAL_START - 14*Mega));
		if (ptr_allocations[13] != NULL) { correct = 0; cprintf("Kmalloc: Attempt to allocate large segment with no suitable fragment to fit on, should return NULL\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

	}
	if (correct)	eval+=10 ;

	cprintf("test FIRST FIT allocation (2) completed. Eval = %d%\n", eval);

	return 1;
}

<<<<<<< HEAD
void* ptr_fast_allocations[(KERNEL_HEAP_MAX - KERNEL_HEAP_START)/PAGE_SIZE] = {0};
int test_fastfirstfit()
{
	/*********************** NOTE ****************************
	 * WE COMPARE THE DIFF IN FREE FRAMES BY "AT LEAST" RULE
	 * INSTEAD OF "EQUAL" RULE SINCE IT'S POSSIBLE FOR SOME
	 * IMPLEMENTATIONS TO DYNAMICALLY ALLOCATE SPECIAL DATA
	 * STRUCTURE TO MANAGE THE PAGE ALLOCATOR.
	 *********************************************************/

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	// allocate pages
	int freeFrames = sys_calculate_free_frames() ;
	int freeDiskFrames = pf_calculate_free_frames() ;

	uint32 allocSize = 8*kilo;
	int i;
	cprintf("\n1\n");
	//ptr_allocations[0] = kmalloc(2*Mega - KERNEL_SHARES_ARR_INIT_SIZE - KERNEL_SEMAPHORES_ARR_INIT_SIZE);
	int numOf2MAllocs = (KERNEL_HEAP_MAX - ACTUAL_START) / allocSize;
	for(i = 0; i < numOf2MAllocs ;i++)
	{
		ptr_fast_allocations[i] = kmalloc(allocSize);
	}
	cprintf("\n2\n");

	uint32 remainSpace = (KERNEL_HEAP_MAX - ACTUAL_START) % allocSize;
	if (remainSpace != 0)
	{
		ptr_fast_allocations[numOf2MAllocs++] = kmalloc(remainSpace);
	}
	cprintf("\nNumber of %x size allocations to fill the page allocator area = %d\n",allocSize, numOf2MAllocs);
	// check the addresses of the allocation
	for(i = 0; i < numOf2MAllocs ;i++)
	{
		if((uint32)ptr_fast_allocations[i] != ACTUAL_START + i*allocSize)
			panic("Wrong allocation, Check next fitting strategy is working correctly");
	}

	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) < (KERNEL_HEAP_MAX - ACTUAL_START)/(PAGE_SIZE) ) panic("Wrong allocation");

	// Make memory holes.
	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;

	if (numOf2MAllocs < 30)
		panic("unexpcected number of allocations! PLEASE REVISE THE STAFF");
	kfree(ptr_fast_allocations[0]);		// Hole 1 = allocSize
	kfree(ptr_fast_allocations[2]);		// Hole 2 = 2*allocSize
	kfree(ptr_fast_allocations[3]);
	kfree(ptr_fast_allocations[5]);		// Hole 3 = allocSize
	kfree(ptr_fast_allocations[10]);		// Hole 4 = 3*allocSize
	kfree(ptr_fast_allocations[12]);
	kfree(ptr_fast_allocations[11]);
	kfree(ptr_fast_allocations[20]);		// Hole 5 = allocSize
	kfree(ptr_fast_allocations[25]);		// Hole 6 = allocSize
	kfree(ptr_fast_allocations[numOf2MAllocs-2]);		// Last Hole 7 = 2*allocSize
	kfree(ptr_fast_allocations[numOf2MAllocs-3]);

	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((sys_calculate_free_frames() - freeFrames) != (11*allocSize)/PAGE_SIZE) panic("Wrong free: Extra or less pages are removed from main memory");

	uint32 desiredSize;
	// Test first fit
	//2.5 x allocSize in Hole 4 -> Hole 4 becomes 0.5 allocSize
	{
		desiredSize = 2*allocSize + allocSize/2 ;
		freeDiskFrames = pf_calculate_free_frames() ;
		freeFrames = sys_calculate_free_frames() ;
		void* tempAddress = kmalloc(desiredSize);
		if((uint32)tempAddress != ACTUAL_START + 10*allocSize)
			panic("First Fit not working correctly");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) < (desiredSize)/PAGE_SIZE) panic("Wrong allocation");
	}

	//2 x allocSize in Hole 2 -> Hole 2 becomes 0
	{
		desiredSize = 2*allocSize ;
		freeDiskFrames = pf_calculate_free_frames() ;
		freeFrames = sys_calculate_free_frames() ;
		void* tempAddress = kmalloc(desiredSize);
		if((uint32)tempAddress != ACTUAL_START + 2*allocSize)
			panic("First Fit not working correctly");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) < (desiredSize)/PAGE_SIZE) panic("Wrong allocation");
	}

	//1.5 x allocSize in Hole 7 -> Hole 7 becomes 0.5 x allocSize
	{
		desiredSize = 3*allocSize/2 ;
		freeDiskFrames = pf_calculate_free_frames() ;
		freeFrames = sys_calculate_free_frames() ;
		void* tempAddress = kmalloc(desiredSize);
		if((uint32)tempAddress != ACTUAL_START + (numOf2MAllocs-3)*allocSize)
			panic("First Fit not working correctly");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) < (desiredSize)/PAGE_SIZE) panic("Wrong allocation");
	}

	//allocSize in Hole 1 -> Hole 1 becomes 0 M
	{
		freeDiskFrames = pf_calculate_free_frames() ;
		freeFrames = sys_calculate_free_frames() ;
		void* tempAddress = kmalloc(allocSize);
		if((uint32)tempAddress != ACTUAL_START + 0*allocSize)
			panic("First Fit not working correctly");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) < (allocSize)/PAGE_SIZE) panic("Wrong allocation");
	}

	//allocSize in Hole 3 -> Hole 3 becomes 0 M
	{
		freeDiskFrames = pf_calculate_free_frames() ;
		freeFrames = sys_calculate_free_frames() ;
		void* tempAddress = kmalloc(allocSize);
		if((uint32)tempAddress != ACTUAL_START + 5*allocSize)
			panic("First Fit not working correctly");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) < (allocSize)/PAGE_SIZE) panic("Wrong allocation");
	}

	//allocSize in Hole 5 -> Hole 5 becomes 0 M
	{
		freeDiskFrames = pf_calculate_free_frames() ;
		freeFrames = sys_calculate_free_frames() ;
		void* tempAddress = kmalloc(allocSize);
		if((uint32)tempAddress != ACTUAL_START + 20*allocSize)
			panic("First Fit not working correctly");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) < (allocSize)/PAGE_SIZE) panic("Wrong allocation");
	}

	//allocSize in Hole 6 -> Hole 6 becomes 0 M
	{
		freeDiskFrames = pf_calculate_free_frames() ;
		freeFrames = sys_calculate_free_frames() ;
		void* tempAddress = kmalloc(allocSize);
		if((uint32)tempAddress != ACTUAL_START + 25*allocSize)
			panic("First Fit not working correctly");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) < (allocSize)/PAGE_SIZE) panic("Wrong allocation");
	}

	//0.5 x allocSize in Remaining of Hole 4 -> Hole 4 becomes 0 M
	{
		desiredSize = allocSize / 2 ;
		freeDiskFrames = pf_calculate_free_frames() ;
		freeFrames = sys_calculate_free_frames() ;
		void* tempAddress = kmalloc(desiredSize);
		if((uint32)tempAddress != ACTUAL_START + 10*allocSize + 5*allocSize/2)
			panic("First Fit not working correctly");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) < (desiredSize)/PAGE_SIZE) panic("Wrong allocation");
	}

	//0.5 x allocSize in Remaining of Hole 7 -> Hole 7 becomes 0 M
	{
		desiredSize = allocSize / 2 ;
		freeDiskFrames = pf_calculate_free_frames() ;
		freeFrames = sys_calculate_free_frames() ;
		void* tempAddress = kmalloc(desiredSize);
		if((uint32)tempAddress != ACTUAL_START + (numOf2MAllocs-3)*allocSize + 3*allocSize/2)
			panic("First Fit not working correctly");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) < (desiredSize)/PAGE_SIZE) panic("Wrong allocation");
	}
	cprintf("\nIf this test finishes in less than 5 SEC... TAKE YOUR BONUS :).\n");
	return 1;

}

=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

int test_kfree_bestfirstfit()
{
	/*********************** NOTE ****************************
	 * WE COMPARE THE DIFF IN FREE FRAMES BY "AT LEAST" RULE
	 * INSTEAD OF "EQUAL" RULE SINCE IT'S POSSIBLE FOR SOME
	 * IMPLEMENTATIONS TO DYNAMICALLY ALLOCATE SPECIAL DATA
	 * STRUCTURE TO MANAGE THE PAGE ALLOCATOR.
	 *********************************************************/

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;
	//	void* expected;

	char *byteArr, *byteArr2 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = sys_calculate_free_frames() ;

	//malloc some spaces
	int i, freeFrames, freeDiskFrames ;
	char* ptr;
	int lastIndices[20] = {0};
	int sums[20] = {0};

	int eval = 0;
	bool correct = 1;

	correct = 1;
	void* ptr_allocations[20] = {0};
<<<<<<< HEAD
	//Create some spaces in both BLOCK & PAGE allocators
	cprintf("\n1. Create some spaces in both BLOCK & PAGE allocators\n");
	{
		/*[PAGE ALLOCATOR]*/
=======
	{
		//[BLOCK ALLOCATOR]
		{
			//2 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[2] = kmalloc(2*kilo);
			if ((uint32) ptr_allocations[2] < KERNEL_HEAP_START || ptr_allocations[2] >= sbrk(0) || (uint32) ptr_allocations[2] >= da_limit)
			{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			//		if ((freeFrames - sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
			lastIndices[2] = (2*kilo)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[2];
			for (i = 0; i < lastIndices[2]; ++i)
			{
				ptr[i] = 2 ;
			}

			//2 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[3] = kmalloc(2*kilo);
			if ((uint32) ptr_allocations[3] < KERNEL_HEAP_START || ptr_allocations[3] >= sbrk(0) || (uint32) ptr_allocations[3] >= da_limit)
			{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			//		if ((freeFrames - sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
			lastIndices[3] = (2*kilo)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[3];
			for (i = 0; i < lastIndices[3]; ++i)
			{
				ptr[i] = 3 ;
			}
		}

		//[PAGE ALLOCATOR]
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		{
			//2 MB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[0] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
			if ((uint32) ptr_allocations[0] !=  (ACTUAL_START)) { correct = 0; cprintf("1.1 Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("1.1 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
			if ((uint32) ptr_allocations[0] !=  (ACTUAL_START)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			lastIndices[0] = (2*Mega-kilo)/sizeof(char) - 1;

			//2 MB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[1] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
			if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("1.2 Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("1.2 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
			if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			lastIndices[1] = (2*Mega-kilo)/sizeof(char) - 1;


			//7 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[4] = kmalloc(7*kilo);
<<<<<<< HEAD
			if ((uint32) ptr_allocations[4] != (ACTUAL_START + 4*Mega /* + 8*kilo*/)) { correct = 0; cprintf("1.3 Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 2) { correct = 0; cprintf("1.3 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
			if ((uint32) ptr_allocations[4] != (ACTUAL_START + 4*Mega /* + 8*kilo*/)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 2) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			lastIndices[4] = (7*kilo)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[4];
			for (i = 0; i < lastIndices[4]; ++i)
			{
				ptr[i] = 4 ;
			}

			//3 MB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[5] = kmalloc(3*Mega-kilo);
<<<<<<< HEAD
			if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega + 8*kilo) ) { correct = 0; cprintf("1.4 Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.4 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 768) { correct = 0; cprintf("1.4 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
			if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega + 8*kilo) ) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 768) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			lastIndices[5] = (3*Mega-kilo)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[5];
			for (i = 0; i < lastIndices[5]; ++i)
			{
				ptr[i] = 5 ;
			}

			//6 MB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[6] = kmalloc(6*Mega-kilo);
<<<<<<< HEAD
			if ((uint32) ptr_allocations[6] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("1.5 Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.5 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 1536) { correct = 0; cprintf("1.5 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
			if ((uint32) ptr_allocations[6] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 1536) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			lastIndices[6] = (6*Mega-kilo)/sizeof(char) - 1;

			//14 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[7] = kmalloc(14*kilo);
<<<<<<< HEAD
			if ((uint32) ptr_allocations[7] != (ACTUAL_START + 13*Mega + 8*kilo)) { correct = 0; cprintf("1.6 Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.6 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 4) { correct = 0; cprintf("1.6 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
			if ((uint32) ptr_allocations[7] != (ACTUAL_START + 13*Mega + 8*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 4) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			lastIndices[7] = (14*kilo)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[7];
			for (i = 0; i < lastIndices[7]; ++i)
			{
				ptr[i] = 7 ;
			}
		}
<<<<<<< HEAD

		/*[BLOCK ALLOCATOR]*/
		freeFrames = sys_calculate_free_frames() ;
		{
			//2 KB
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[2] = kmalloc(2*kilo);
			if ((uint32) ptr_allocations[2] < KERNEL_HEAP_START || ptr_allocations[2] >= sbrk(0) || (uint32) ptr_allocations[2] >= da_limit)
			{ correct = 0; cprintf("1.7 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.7 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			lastIndices[2] = (2*kilo)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[2];
			for (i = 0; i < lastIndices[2]; ++i)
			{
				ptr[i] = 2 ;
			}

			//2 KB
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[3] = kmalloc(2*kilo);
			if ((uint32) ptr_allocations[3] < KERNEL_HEAP_START || ptr_allocations[3] >= sbrk(0) || (uint32) ptr_allocations[3] >= da_limit)
			{ correct = 0; cprintf("1.8 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.8 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			lastIndices[3] = (2*kilo)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[3];
			for (i = 0; i < lastIndices[3]; ++i)
			{
				ptr[i] = 3 ;
			}
		}
		if ((freeFrames - sys_calculate_free_frames()) != 1) { correct = 0; cprintf("1.8 Wrong allocation: sbrk error\n"); }
	}

	//kfree some of the allocated spaces [10%]
	cprintf("\n2. kfree some of the allocated spaces [10%]\n");
=======
	}

	//kfree some of the allocated spaces [10%]
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		//kfree 1st 2 MB
		int freeFrames = sys_calculate_free_frames() ;
		int freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
<<<<<<< HEAD
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("2.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512 ) { correct = 0; cprintf("2.1 Wrong kfree: pages in memory are not freed correctly\n"); }

		//kfree 1st 2 KB from BLOCK Allocator
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[2]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("2.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) != 0 ) { correct = 0; cprintf("2.2 Wrong free: freeing a block from the dynamic allocator should not affect the free frames\n"); }
=======
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512 ) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }

		//kfree 1st 2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[2]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) != 0 ) { correct = 0; cprintf("Wrong free: freeing a block from the dynamic allocator should not affect the free frames\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//kfree 2nd 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
<<<<<<< HEAD
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("2.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("2.3 Wrong kfree: pages in memory are not freed correctly\n"); }
=======
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//kfree 6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[6]);
<<<<<<< HEAD
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("2.4 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 6*Mega/4096) { correct = 0; cprintf("2.4 Wrong kfree: pages in memory are not freed correctly\n"); }
=======
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 6*Mega/4096) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=10 ;

	correct = 1 ;
<<<<<<< HEAD
	//Check memory access of non-freed areas in both allocators [10%]
	cprintf("\n3. Check memory access of non-freed areas in both allocators [10%]\n");
=======
	//Check memory access after kfree [10%]
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		//2 KB
		ptr = (char*)ptr_allocations[3];
		for (i = 0; i < lastIndices[3]; ++i)
		{
			sums[3] += ptr[i] ;
		}
<<<<<<< HEAD
		if (sums[3] != 3*lastIndices[3])	{ correct = 0; cprintf("3.1 kfree: invalid read after freeing some allocations\n"); }
=======
		if (sums[3] != 3*lastIndices[3])	{ correct = 0; cprintf("kfree: invalid read after freeing some allocations\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//7 KB
		ptr = (char*)ptr_allocations[4];
		for (i = 0; i < lastIndices[4]; ++i)
		{
			sums[4] += ptr[i] ;
		}
<<<<<<< HEAD
		if (sums[4] != 4*lastIndices[4])	{ correct = 0; cprintf("3.2 kfree: invalid read after freeing some allocations\n"); }
=======
		if (sums[4] != 4*lastIndices[4])	{ correct = 0; cprintf("kfree: invalid read after freeing some allocations\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//3 MB
		ptr = (char*)ptr_allocations[5];
		for (i = 0; i < lastIndices[5]; ++i)
		{
			sums[5] += ptr[i] ;
		}
<<<<<<< HEAD
		if (sums[5] != 5*lastIndices[5])	{ correct = 0; cprintf("3.3 kfree: invalid read after freeing some allocations\n"); }
=======
		if (sums[5] != 5*lastIndices[5])	{ correct = 0; cprintf("kfree: invalid read after freeing some allocations\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//14 KB
		ptr = (char*)ptr_allocations[7];
		for (i = 0; i < lastIndices[7]; ++i)
		{
			sums[7] += ptr[i] ;
		}
<<<<<<< HEAD
		if (sums[7] != 7*lastIndices[7])	{ correct = 0; cprintf("3.4 kfree: invalid read after freeing some allocations\n"); }
=======
		if (sums[7] != 7*lastIndices[7])	{ correct = 0; cprintf("kfree: invalid read after freeing some allocations\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=10 ;

	correct = 1 ;
	//Allocate after kfree [15%]
<<<<<<< HEAD
	cprintf("\n4. Allocate after kfree [15%]\n");
	{
		//1 KB [Should be allocated in 1st hole in the Dynamic Allocator]
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[9] = kmalloc(1*kilo);
		if (ptr_allocations[9] != ptr_allocations[2])
		{ correct = 0; cprintf("4.1 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("4.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) != 0) { correct = 0; cprintf("4.1 Wrong allocation: it's allocated in a previously allocated block. Should not allocate any pages from physical memory\n"); }
		lastIndices[9] = (1*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[9];
		for (i = 0; i < lastIndices[9]; ++i)
		{
			ptr[i] = 9 ;
		}

=======
	{
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		//Allocate in merged freed space
		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[8] = kmalloc(3*Mega);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[8] != (ACTUAL_START)) { correct = 0; cprintf("4.2 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("4.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 768) { correct = 0; cprintf("4.2 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[8] != (ACTUAL_START)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 768) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		lastIndices[8] = (3*Mega)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[8];
		for (i = 0; i < lastIndices[8]; ++i)
		{
			ptr[i] = 8 ;
		}

		//1 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[10] = kmalloc(1*Mega);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[10] != (ACTUAL_START + 3*Mega /*+ 4*kilo*/)) { correct = 0; cprintf("4.3 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("4.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 256) { correct = 0; cprintf("4.3 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[10] != (ACTUAL_START + 3*Mega /*+ 4*kilo*/)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 256) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		lastIndices[10] = (1*Mega)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[10];
		for (i = 0; i < lastIndices[10]; ++i)
		{
			ptr[i] = 10 ;
		}
<<<<<<< HEAD
=======

		//1 KB [Should be allocated in 1st hole in the Dynamic Allocator]
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[9] = kmalloc(1*kilo);
		if ((ptr_allocations[9] < ptr_allocations[2]) || (ptr_allocations[9] > (ptr_allocations[2] + 1*kilo)))
		{ correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) != 0) { correct = 0; cprintf("Wrong allocation: it's allocated in a previously allocated block. Should not allocate any pages from physical memory\n"); }
		lastIndices[9] = (1*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[9];
		for (i = 0; i < lastIndices[9]; ++i)
		{
			ptr[i] = 9 ;
		}

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=15 ;

	correct = 1 ;
	//kfree remaining allocated spaces [15%]
<<<<<<< HEAD
	cprintf("\n5. kfree remaining allocated spaces [15%]\n");
	{
		/*[PAGE ALLOCATOR]*/
		{
			//kfree 3 MB [PAGE ALLOCATOR: Should be Merged with NEXT 6 MB hole - total = 9MB]
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			kfree(ptr_allocations[5]);
			if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("5.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((sys_calculate_free_frames() - freeFrames) < 3*Mega/4096) { correct = 0; cprintf("5.1 Wrong kfree: pages in memory are not freed correctly\n"); }

			//kfree 7 KB [PAGE ALLOCATOR: Should be Merged with NEXT 9 MB hole - total = 9MB + 8KB]
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			kfree(ptr_allocations[4]);
			if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("5.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((sys_calculate_free_frames() - freeFrames) < 2) { correct = 0; cprintf("5.2 Wrong kfree: pages in memory are not freed correctly\n"); }

			//kfree 14 KB [PAGE ALLOCATOR: Should be Merged with PREV 9MB + 8KB hole - total = 9MB + 24KB]
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			kfree(ptr_allocations[7]);
			if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("5.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((sys_calculate_free_frames() - freeFrames) < 4) { correct = 0; cprintf("5.3 Wrong kfree: pages in memory are not freed correctly\n"); }

			//kfree 1 MB [PAGE ALLOCATOR: Should be Merged with NEXT remaining hole ]
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			kfree(ptr_allocations[10]);
			if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("5.4 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((sys_calculate_free_frames() - freeFrames) < 1*Mega/4096) { correct = 0; cprintf("5.4 Wrong kfree: pages in memory are not freed correctly\n"); }

			//kfree 3 MB [PAGE ALLOCATOR: Should be Merged with PREV 9MB + 24KB hole & NEXT remaining hole - total = ALL PAGE ALLOCATOR Space]
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			kfree(ptr_allocations[8]);
			if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("5.5 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((sys_calculate_free_frames() - freeFrames) < 3*Mega/4096) { correct = 0; cprintf("5.5 Wrong kfree: pages in memory are not freed correctly\n"); }
		}
		/*[BLOCK ALLOCATOR]*/
		{
			//kfree 1 KB [DYNAMIC ALLOCATOR]
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			kfree(ptr_allocations[9]);
			if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("5.6 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("5.6 Wrong kfree: pages in memory are not freed correctly\n"); }

			//kfree 2nd 2 KB [DYNAMIC ALLOCATOR: Should be Merged with PREV remaining area of 2KB & NEXT free space]
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			kfree(ptr_allocations[3]);
			if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("5.7 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("5.7 Wrong free: freeing a block from the dynamic allocator should not affect the free frames\n"); }
		}
=======
	{
		//kfree 3 MB [PAGE ALLOCATOR: Should be Merged with NEXT 6 MB hole - total = 9MB]
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[5]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 3*Mega/4096) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }

		//kfree 7 KB [PAGE ALLOCATOR: Should be Merged with NEXT 9 MB hole - total = 9MB + 8KB]
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[4]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 2) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }

		//kfree 1 KB [DYNAMIC ALLOCATOR]
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[9]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }

		//kfree 2nd 2 KB [DYNAMIC ALLOCATOR: Should be Merged with PREV remaining area of 2KB & NEXT free space]
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[3]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("Wrong free: freeing a block from the dynamic allocator should not affect the free frames\n"); }

		//kfree 14 KB [PAGE ALLOCATOR: Should be Merged with PREV 9MB + 8KB hole - total = 9MB + 24KB]
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[7]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 4) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }

		//kfree 1 MB [PAGE ALLOCATOR: Should be Merged with NEXT remaining hole ]
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[10]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 1*Mega/4096) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }

		//kfree 3 MB [PAGE ALLOCATOR: Should be Merged with PREV 9MB + 24KB hole & NEXT remaining hole - total = ALL PAGE ALLOCATOR Space]
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[8]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 3*Mega/4096) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		//				if(start_freeFrames != (sys_calculate_free_frames())) {{ correct = 0; cprintf("Wrong kfree: not all pages removed correctly at end\n"); }}
	}
	if (correct)	eval+=15 ;

	correct = 1 ;
<<<<<<< HEAD
	//Check memory access of FREED area in PAGE allocator [15%]
	cprintf("\n6. Check memory access of FREED area in PAGE allocator [15%]\n");
=======
	//Check memory access after kfree [15%]
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		//Bypass the PAGE FAULT on <MOVB immediate, reg> instruction by setting its length
		//and continue executing the remaining code
		sys_bypassPageFault(3);

		for (i = 0; i <= 10; ++i)
		{
			//SKIP CHECKING THOSE IN DYNAMIC ALLOCATOR AREA
			if (i == 2 || i == 3 || i == 9)
			{
				continue;
			}
			ptr = (char *) ptr_allocations[i];
			ptr[0] = 10;
			//cprintf("\n\ncr2 = %x, faulted addr = %x", sys_rcr2(), (uint32)&(ptr[0]));
			if (sys_rcr2() != (uint32)&(ptr[0]))
				if (correct)
<<<<<<< HEAD
				{ correct = 0; cprintf("6.1 kfree: successful access to freed space!! it should not be succeeded\n"); }
			ptr[lastIndices[i]] = 10;
			if (sys_rcr2() != (uint32)&(ptr[lastIndices[i]]))
				if (correct)
				{ correct = 0; cprintf("6.2 kfree: successful access to freed space!! it should not be succeeded\n"); }
=======
				{ correct = 0; cprintf("kfree: successful access to freed space!! it should not be succeeded\n"); }
			ptr[lastIndices[i]] = 10;
			if (sys_rcr2() != (uint32)&(ptr[lastIndices[i]]))
				if (correct)
				{ correct = 0; cprintf("kfree: successful access to freed space!! it should not be succeeded\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		}

		//set it to 0 again to cancel the bypassing option
		sys_bypassPageFault(0);
	}
	if (correct)	eval+=15 ;

	correct = 1 ;

<<<<<<< HEAD
	void* expected;
	//Allocate after kfree ALL [30%]
	cprintf("\n7. Allocate after kfree ALL [30%]\n");
	{
		/*[DYNAMIC ALLOCATOR] Allocate in merged freed space*/
		{
			//1 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[11] = kmalloc(1*kilo);
			if (ptr_allocations[11] != ptr_allocations[2])
			{ correct = 0; cprintf("7.1 Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("7.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) != 0) { correct = 0; cprintf("7.1 Wrong allocation: it's allocated in a previously allocated block. Should not allocate any pages from physical memory\n"); }
			lastIndices[11] = (1*kilo)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[11];
			for (i = 0; i < lastIndices[11]; ++i)
			{
				ptr[i] = 11 ;
			}

			//2 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[12] = kmalloc(2*kilo);
			expected = ptr_allocations[2] + 1*kilo + 2*sizeof(int);
			if (ptr_allocations[12] != expected)
			{
				correct = 0;
				cprintf("7.2 Wrong start address for the allocated space... check return address of kmalloc. Expected [%x, %x], Actual %x\n", (ptr_allocations[2] + 1*kilo), (ptr_allocations[2] + 2*kilo), ptr_allocations[12]);
			}
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("7.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) != 0) { correct = 0; cprintf("7.2 Wrong allocation: it's allocated in a previously allocated block. Should not allocate any pages from physical memory\n"); }
			lastIndices[12] = (2*kilo)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[12];
			for (i = 0; i < lastIndices[12]; ++i)
			{
				ptr[i] = 12 ;
			}

			//1.5 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[13] = kmalloc(3*kilo/2);
			expected = ptr_allocations[2] + 1*kilo + 2*sizeof(int) + 2*kilo + 2*sizeof(int);
			if (ptr_allocations[13] != expected)
			{ correct = 0; cprintf("7.3 Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("7.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) != 0) { correct = 0; cprintf("7.3 Wrong allocation: it's allocated in a previously allocated block. Should not allocate any pages from physical memory\n"); }
			lastIndices[13] = (3*kilo/2)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[13];
			for (i = 0; i < lastIndices[13]; ++i)
			{
				ptr[i] = 13 ;
			}
		}

		/*[PAGE ALLOCATOR] Allocate in merged freed space*/
		{
			//30 MB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[10] = kmalloc(30*Mega);
			if ((uint32) ptr_allocations[10] != (ACTUAL_START)) { correct = 0; cprintf("7.4 Wrong start address for the allocated space... check return address of kmalloc\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("7.4 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			if ((freeFrames - sys_calculate_free_frames()) < 30*Mega/PAGE_SIZE) { correct = 0; cprintf("7.4 Wrong allocation: pages are not loaded successfully into memory\n"); }
			lastIndices[10] = (30*Mega)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[10];
			for (i = 0; i < lastIndices[10]; ++i)
			{
				ptr[i] = 10 ;
			}
		}

		/*Check Content*/
		{
			//30 MB
			ptr = (char*)ptr_allocations[10];
			for (i = 0; i < lastIndices[10]; ++i)
			{
				sums[10] += ptr[i] ;
			}
			if (sums[10] != 10*lastIndices[10])	{ correct = 0; cprintf("7.5 kfree: invalid read - data is corrupted\n"); }

			//1 KB
			ptr = (char*)ptr_allocations[11];
			for (i = 0; i < lastIndices[11]; ++i)
			{
				sums[11] += ptr[i] ;
			}
			if (sums[11] != 11*lastIndices[11])	{ correct = 0; cprintf("7.6 kfree: invalid read - data is corrupted\n"); }

			//2 KB
			ptr = (char*)ptr_allocations[12];
			for (i = 0; i < lastIndices[12]; ++i)
			{
				sums[12] += ptr[i] ;
			}
			if (sums[12] != 12*lastIndices[12])	{ correct = 0; cprintf("7.7 kfree: invalid read - data is corrupted\n"); }

			//1.5 KB
			ptr = (char*)ptr_allocations[13];
			for (i = 0; i < lastIndices[13]; ++i)
			{
				sums[13] += ptr[i] ;
			}
			if (sums[13] != 13*lastIndices[13])	{ correct = 0; cprintf("7.8 kfree: invalid read - data is corrupted\n"); }
		}
=======
	//	//kfree non-exist item [10%]
	//	{
	//		//kfree 2 MB
	//		freeFrames = sys_calculate_free_frames() ;
	//		freeDiskFrames = pf_calculate_free_frames() ;
	//		kfree(ptr_allocations[0]);
	//		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
	//		if ((sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing\n"); }
	//
	//		//kfree 2 KB
	//		freeFrames = sys_calculate_free_frames() ;
	//		freeDiskFrames = pf_calculate_free_frames() ;
	//		kfree(ptr_allocations[2]);
	//		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
	//		if ((sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing\n"); }
	//
	//		//kfree 20 KB
	//		freeFrames = sys_calculate_free_frames() ;
	//		freeDiskFrames = pf_calculate_free_frames() ;
	//		kfree(ptr_allocations[8]);
	//		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
	//		if ((sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing\n"); }
	//
	//		//kfree 1 MB
	//		freeFrames = sys_calculate_free_frames() ;
	//		freeDiskFrames = pf_calculate_free_frames() ;
	//		kfree(ptr_allocations[9]);
	//		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
	//		if ((sys_calculate_free_frames() - freeFrames) != 0) { correct = 0; cprintf("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing\n"); }
	//
	//	}
	//	cprintf("\b\b\b75%\n"); }

	//Allocate after kfree ALL [30%]
	{
		//[DYNAMIC ALLOCATOR] Allocate in merged freed space
		//1 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[11] = kmalloc(1*kilo);
		if ((ptr_allocations[11] < ptr_allocations[2]) || (ptr_allocations[11] > (ptr_allocations[2] + 1*kilo)))
		{ correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) != 0) { correct = 0; cprintf("Wrong allocation: it's allocated in a previously allocated block. Should not allocate any pages from physical memory\n"); }
		lastIndices[11] = (1*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[11];
		for (i = 0; i < lastIndices[11]; ++i)
		{
			ptr[i] = 11 ;
		}

		//[DYNAMIC ALLOCATOR] Allocate in merged freed space
		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[12] = kmalloc(2*kilo);
		//expected = ptr_allocations[2] + 1*kilo + sizeOfMetaData();
		//if (ptr_allocations[12] != expected)
		if ((ptr_allocations[12] < ptr_allocations[2] + 1*kilo) || (ptr_allocations[12] > (ptr_allocations[2] + 2*kilo)))
		{
			correct = 0;
			cprintf("Wrong start address for the allocated space... check return address of kmalloc. Expected [%x, %x], Actual %x\n", (ptr_allocations[2] + 1*kilo), (ptr_allocations[2] + 2*kilo), ptr_allocations[12]);
		}
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) != 0) { correct = 0; cprintf("Wrong allocation: it's allocated in a previously allocated block. Should not allocate any pages from physical memory\n"); }
		lastIndices[12] = (2*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[12];
		for (i = 0; i < lastIndices[12]; ++i)
		{
			ptr[i] = 12 ;
		}

		//[DYNAMIC ALLOCATOR] Allocate in merged freed space
		//1.5 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[13] = kmalloc(3*kilo/2);
		//if (ptr_allocations[13] != ptr_allocations[12] + 2*kilo + sizeOfMetaData())
		if ((ptr_allocations[13] < ptr_allocations[2] + 3*kilo) || (ptr_allocations[13] > (ptr_allocations[2] + 4*kilo)))
		{ correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) != 0) { correct = 0; cprintf("Wrong allocation: it's allocated in a previously allocated block. Should not allocate any pages from physical memory\n"); }
		lastIndices[13] = (3*kilo/2)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[13];
		for (i = 0; i < lastIndices[13]; ++i)
		{
			ptr[i] = 13 ;
		}

		//[PAGE ALLOCATOR] Allocate in merged freed space
		//30 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[10] = kmalloc(30*Mega);
		if ((uint32) ptr_allocations[10] != (ACTUAL_START)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 30*Mega/PAGE_SIZE) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
		lastIndices[10] = (30*Mega)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[10];
		for (i = 0; i < lastIndices[10]; ++i)
		{
			ptr[i] = 10 ;
		}


		//30 MB
		ptr = (char*)ptr_allocations[10];
		for (i = 0; i < lastIndices[10]; ++i)
		{
			sums[10] += ptr[i] ;
		}
		if (sums[10] != 10*lastIndices[10])	{ correct = 0; cprintf("kfree: invalid read - data is corrupted\n"); }

		//1 KB
		ptr = (char*)ptr_allocations[11];
		for (i = 0; i < lastIndices[11]; ++i)
		{
			sums[11] += ptr[i] ;
		}
		if (sums[11] != 11*lastIndices[11])	{ correct = 0; cprintf("kfree: invalid read - data is corrupted\n"); }

		//2 KB
		ptr = (char*)ptr_allocations[12];
		for (i = 0; i < lastIndices[12]; ++i)
		{
			sums[12] += ptr[i] ;
		}
		if (sums[12] != 12*lastIndices[12])	{ correct = 0; cprintf("kfree: invalid read - data is corrupted\n"); }

		//1.5 KB
		ptr = (char*)ptr_allocations[13];
		for (i = 0; i < lastIndices[13]; ++i)
		{
			sums[13] += ptr[i] ;
		}
		if (sums[13] != 13*lastIndices[13])	{ correct = 0; cprintf("kfree: invalid read - data is corrupted\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}
	if (correct)	eval+=30 ;

	correct = 1 ;
	//check tables	[5%]
<<<<<<< HEAD
	cprintf("\n8. check page tables [5%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		long long va;
		for (va = KERNEL_HEAP_START; va < (long long)KERNEL_HEAP_MAX; va+=PTSIZE)
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, (uint32)va, &ptr_table);
			if (ptr_table == NULL)
			{
				if (correct)
<<<<<<< HEAD
				{ correct = 0; cprintf("8 Wrong kfree: one of the kernel tables is wrongly removed! Tables should not be removed here in kfree\n"); }
=======
				{ correct = 0; cprintf("Wrong kfree: one of the kernel tables is wrongly removed! Tables should not be removed here in kfree\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			}
		}
	}
	if (correct)	eval+=5 ;

	cprintf("\ntest kfree completed. Eval = %d%\n", eval);

	return 1;

}

int test_kheap_phys_addr()
{
	/*********************** NOTE ****************************
	 * WE COMPARE THE DIFF IN FREE FRAMES BY "AT LEAST" RULE
	 * INSTEAD OF "EQUAL" RULE SINCE IT'S POSSIBLE FOR SOME
	 * IMPLEMENTATIONS TO DYNAMICALLY ALLOCATE SPECIAL DATA
	 * STRUCTURE TO MANAGE THE PAGE ALLOCATOR.
	 *********************************************************/

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	char *byteArr, *byteArr2 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = sys_calculate_free_frames() ;

	//malloc some spaces
<<<<<<< HEAD
	cprintf("\n1. Allocate some spaces in both allocators \n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	int i, freeFrames, freeDiskFrames ;
	char* ptr;
	int lastIndices[20] = {0};
	int sums[20] = {0};
	int eval = 0;
	bool correct = 1;
	void* ptr_allocations[20] = {0};
	{
		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[0] !=  (ACTUAL_START)) { correct = 0; cprintf("1.1 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("1.1 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[0] !=  (ACTUAL_START)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("1.2 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("1.2 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//[DYNAMIC ALLOCATOR]
		{
			//1 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[2] = kmalloc(1*kilo);
			if ((uint32) ptr_allocations[2] < KERNEL_HEAP_START || ptr_allocations[2] >= sbrk(0) || (uint32) ptr_allocations[2] >= da_limit)
<<<<<<< HEAD
			{ correct = 0; cprintf("1.3 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
=======
			{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			//if ((freeFrames - sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

			//2 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[3] = kmalloc(2*kilo);
			if ((uint32) ptr_allocations[3] < KERNEL_HEAP_START || ptr_allocations[3] >= sbrk(0) || (uint32) ptr_allocations[3] >= da_limit)
<<<<<<< HEAD
			{ correct = 0; cprintf("1.4 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.4 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
=======
			{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			//if ((freeFrames - sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

			//1.5 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[4] = kmalloc(3*kilo/2);
			if ((uint32) ptr_allocations[4] < KERNEL_HEAP_START || ptr_allocations[4] >= sbrk(0) || (uint32) ptr_allocations[4] >= da_limit)
<<<<<<< HEAD
			{ correct = 0; cprintf("1.5 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.5 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
=======
			{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		}

		//7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(7*kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega /*+ 8*kilo*/)) { correct = 0; cprintf("1.6 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.6 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 2) { correct = 0; cprintf("1.6 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega /*+ 8*kilo*/)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 2) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(3*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 4*Mega + 8*kilo) ) { correct = 0; cprintf("1.7 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.7 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 768) { correct = 0; cprintf("1.7 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 4*Mega + 8*kilo) ) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 768) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(6*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("1.8 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.8 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 1536) { correct = 0; cprintf("1.8 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 1536) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//14 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[8] = kmalloc(14*kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 13*Mega + 8*kilo)) { correct = 0; cprintf("1.9 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.9 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 4) { correct = 0; cprintf("1.9 Wrong allocation: pages are not loaded successfully into memory\n"); }
	}

	//[PAGE ALLOCATOR] test kheap_physical_address after kmalloc only [30%]
	cprintf("\n2. [PAGE ALLOCATOR] test kheap_physical_address after kmalloc only [30%]\n");
=======
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 13*Mega + 8*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 4) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
	}

	//[PAGE ALLOCATOR] test kheap_physical_address after kmalloc only [30%]
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		uint32 va;
		uint32 endVA = ACTUAL_START + 13*Mega + 24*kilo;
		uint32 allPAs[(13*Mega + 24*kilo + INITIAL_KHEAP_ALLOCATIONS)/PAGE_SIZE] ;
		i = 0;
		uint32 offset = 1;
		uint32 startVA = da_limit + PAGE_SIZE;
		for (va = startVA; va < endVA; va+=PAGE_SIZE+offset)
		{
			allPAs[i++] = kheap_physical_address(va);
		}
		int ii = i ;
		i = 0;
		int j;
		for (va = startVA; va < endVA; )
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
<<<<<<< HEAD
			{ correct = 0; panic("2.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
=======
			{ correct = 0; panic("one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

			for (j = PTX(va); i < ii && j < 1024 && va < endVA; ++j, ++i)
			{
				if (((ptr_table[j] & 0xFFFFF000)+(va & 0x00000FFF))!= allPAs[i])
				{
					//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					if (correct)
<<<<<<< HEAD
					{ correct = 0; cprintf("2.2 Wrong kheap_physical_address\n"); }
=======
					{ correct = 0; cprintf("Wrong kheap_physical_address\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
				}
				va+=PAGE_SIZE+offset;
			}
		}
	}
	if (correct)	eval+=30 ;

	correct = 1 ;
	//[DYNAMIC ALLOCATOR] test kheap_physical_address after kmalloc only [10%]
<<<<<<< HEAD
	cprintf("\n3. [DYNAMIC ALLOCATOR] test kheap_physical_address after kmalloc only [10%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		int i;
		uint32 va, pa;
		for (i = 2; i <= 4; i++)
		{
			va = (uint32)ptr_allocations[i];
			pa = kheap_physical_address(va);
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
<<<<<<< HEAD
			{ correct = 0; panic("3.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
=======
			{ correct = 0; panic("one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

			if (((ptr_table[PTX(va)] & 0xFFFFF000)+(va & 0x00000FFF))!= pa)
			{
				//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
				if (correct)
<<<<<<< HEAD
				{ correct = 0; cprintf("3.2 Wrong kheap_physical_address\n"); }
=======
				{ correct = 0; cprintf("Wrong kheap_physical_address\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			}
		}
	}
	if (correct)	eval+=10 ;

	correct = 1 ;
	//kfree some of the allocated spaces
<<<<<<< HEAD
	cprintf("\n4. kfree some of the allocated spaces\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		//kfree 1st 2 MB
		int freeFrames = sys_calculate_free_frames() ;
		int freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
<<<<<<< HEAD
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("4.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512 ) { correct = 0; cprintf("4.1 Wrong kfree: pages in memory are not freed correctly\n"); }
=======
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512 ) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//kfree 2nd 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
<<<<<<< HEAD
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("4.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("4.2 Wrong kfree: pages in memory are not freed correctly\n"); }
=======
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//kfree 6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[7]);
<<<<<<< HEAD
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("4.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 6*Mega/4096) { correct = 0; cprintf("4.3 Wrong kfree: pages in memory are not freed correctly\n"); }
	}

	uint32 expected;
	//[PAGE ALLOCATOR] test kheap_physical_address after kmalloc and kfree [20%]
	cprintf("\n5. [PAGE ALLOCATOR] test kheap_physical_address after kmalloc and kfree [20%]\n");
=======
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 6*Mega/4096) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }
	}

	//[PAGE ALLOCATOR] test kheap_physical_address after kmalloc and kfree [20%]
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		uint32 va;
		uint32 endVA = ACTUAL_START + 13*Mega + 24*kilo;
		uint32 allPAs[(13*Mega + 24*kilo + INITIAL_KHEAP_ALLOCATIONS)/PAGE_SIZE] ;
		i = 0;
<<<<<<< HEAD
		uint32 offset = 121;
		uint32 startVA = da_limit + PAGE_SIZE+offset;
=======
		uint32 startVA = da_limit + PAGE_SIZE;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		for (va = startVA; va < endVA; va+=PAGE_SIZE)
		{
			allPAs[i++] = kheap_physical_address(va);
		}
		int ii = i ;
		i = 0;
		int j;
		for (va = startVA; va < endVA; )
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
				if (correct)
<<<<<<< HEAD
				{ correct = 0; panic("5.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }

			for (j = PTX(va); i < ii && j < 1024 && va < endVA; ++j, ++i)
			{
				expected = 0 ;
				if ((ptr_table[j] & PERM_PRESENT))
				{
					expected = (ptr_table[j] & 0xFFFFF000) + (va & 0x00000FFF);
				}
				//if (((ptr_table[j] & 0xFFFFF000)+((ptr_table[j] & PERM_PRESENT) == 0? 0 : va & 0x00000FFF)) != allPAs[i])
				if (expected != allPAs[i])
				{
					//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					if (correct)
					{ correct = 0; cprintf("5.2 Wrong kheap_physical_address\n"); }
=======
				{ correct = 0; panic("one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }

			for (j = PTX(va); i < ii && j < 1024 && va < endVA; ++j, ++i)
			{
				if (((ptr_table[j] & 0xFFFFF000)+((ptr_table[j] & PERM_PRESENT) == 0? 0 : va & 0x00000FFF)) != allPAs[i])
				{
					//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					if (correct)
					{ correct = 0; cprintf("Wrong kheap_physical_address\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
				}
				va += PAGE_SIZE;
			}
		}
	}
	if (correct)	eval+=20 ;

	correct = 1 ;
	//[DYNAMIC ALLOCATOR] test kheap_physical_address on the entire allocated area [30%]
<<<<<<< HEAD
	cprintf("\n6. [DYNAMIC ALLOCATOR] test kheap_physical_address on the entire allocated area [30%]\n");
	{
		uint32 va, pa;
		if ((uint32)sbrk(0) <= KERNEL_HEAP_START + PAGE_SIZE) panic("6.1 unexpected sbrk value");
=======
	{
		uint32 va, pa;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		for (va = KERNEL_HEAP_START; va < (uint32)sbrk(0); va++)
		{
			pa = kheap_physical_address(va);
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
				if (correct)
<<<<<<< HEAD
				{ correct = 0; panic("6.2 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
=======
				{ correct = 0; panic("one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

			if (((ptr_table[PTX(va)] & 0xFFFFF000)+(va & 0x00000FFF))!= pa)
			{
				//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
				if (correct)
<<<<<<< HEAD
				{ correct = 0; cprintf("6.3 Wrong kheap_physical_address\n"); }
=======
				{ correct = 0; cprintf("Wrong kheap_physical_address\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			}
		}
	}
	if (correct)	eval+=30 ;

	correct = 1 ;
	//test kheap_physical_address on non-mapped area [10%]
<<<<<<< HEAD
	cprintf("\n7. test kheap_physical_address on non-mapped area [10%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		uint32 va;
		uint32 startVA = ACTUAL_START + 16*Mega;
		i = 0;
		for (va = startVA; va < KERNEL_HEAP_MAX; va+=PAGE_SIZE)
		{
			i++;
		}
		int ii = i ;
		i = 0;
		int j;
		long long va2;
		for (va2 = startVA; va2 < (long long)KERNEL_HEAP_MAX; va2+=PTSIZE)
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, (uint32)va2, &ptr_table);
			if (ptr_table == NULL)
			{
				if (correct)
<<<<<<< HEAD
				{ correct = 0; panic("7.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
=======
				{ correct = 0; panic("one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			}
			for (j = 0; i < ii && j < 1024; ++j, ++i)
			{
				//if ((ptr_table[j] & 0xFFFFF000) != allPAs[i])
				unsigned int page_va = startVA+i*PAGE_SIZE;
				unsigned int supposed_kheap_phys_add = kheap_physical_address(page_va);
<<<<<<< HEAD
				//if (((ptr_table[j] & 0xFFFFF000)+((ptr_table[j] & PERM_PRESENT) == 0? 0 : page_va & 0x00000FFF)) != supposed_kheap_phys_add)
				if (supposed_kheap_phys_add != 0)
				{
					//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va2 + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					if (correct)
					{ correct = 0; cprintf("7.2 Wrong kheap_physical_address\n"); }
=======
				if (((ptr_table[j] & 0xFFFFF000)+((ptr_table[j] & PERM_PRESENT) == 0? 0 : page_va & 0x00000FFF)) != supposed_kheap_phys_add)
				{
					//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va2 + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					if (correct)
					{ correct = 0; cprintf("Wrong kheap_physical_address\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
				}
			}
		}
	}
	if (correct)	eval+=10 ;

	cprintf("\ntest kheap_physical_address completed. Eval = %d%\n", eval);

	return 1;

}

int test_kheap_virt_addr()
{
	/*********************** NOTE ****************************
	 * WE COMPARE THE DIFF IN FREE FRAMES BY "AT LEAST" RULE
	 * INSTEAD OF "EQUAL" RULE SINCE IT'S POSSIBLE FOR SOME
	 * IMPLEMENTATIONS TO DYNAMICALLY ALLOCATE SPECIAL DATA
	 * STRUCTURE TO MANAGE THE PAGE ALLOCATOR.
	 *********************************************************/

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	char *byteArr, *byteArr2 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = sys_calculate_free_frames() ;

	//malloc some spaces
<<<<<<< HEAD
	cprintf("\n1. Allocate some spaces in both allocators \n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	int i, freeFrames, freeDiskFrames ;
	char* ptr;
	int lastIndices[20] = {0};
	int sums[20] = {0};

	int eval = 0;
	bool correct = 1;

	void* ptr_allocations[20] = {0};
	{
		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[0] !=  (ACTUAL_START)) { correct = 0; cprintf("1.1 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("1.1 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[0] !=  (ACTUAL_START)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("1.2 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("1.2 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//[DYNAMIC ALLOCATOR]
		{
			//1 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[2] = kmalloc(1*kilo);
			if ((uint32) ptr_allocations[2] < KERNEL_HEAP_START || ptr_allocations[2] >= sbrk(0) || (uint32) ptr_allocations[2] >= da_limit)
<<<<<<< HEAD
			{ correct = 0; cprintf("1.3 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
=======
			{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			//if ((freeFrames - sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

			//2 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[3] = kmalloc(2*kilo);
			if ((uint32) ptr_allocations[3] < KERNEL_HEAP_START || ptr_allocations[3] >= sbrk(0) || (uint32) ptr_allocations[3] >= da_limit)
<<<<<<< HEAD
			{ correct = 0; cprintf("1.4 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.4 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
=======
			{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			//if ((freeFrames - sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

			//1.5 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[4] = kmalloc(3*kilo/2);
			if ((uint32) ptr_allocations[4] < KERNEL_HEAP_START || ptr_allocations[4] >= sbrk(0) || (uint32) ptr_allocations[4] >= da_limit)
<<<<<<< HEAD
			{ correct = 0; cprintf("1.5 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.5 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
=======
			{ correct = 0; cprintf("Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		}

		//7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(7*kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega /*+ 8*kilo*/)) { correct = 0; cprintf("1.6 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.6 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 2) { correct = 0; cprintf("1.6 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega /*+ 8*kilo*/)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 2) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(3*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 4*Mega + 8*kilo) ) { correct = 0; cprintf("1.7 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.7 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 768) { correct = 0; cprintf("1.7 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 4*Mega + 8*kilo) ) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 768) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(6*Mega-kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("1.8 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.8 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 1536) { correct = 0; cprintf("1.8 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 1536) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//14 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[8] = kmalloc(14*kilo);
<<<<<<< HEAD
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 13*Mega + 8*kilo)) { correct = 0; cprintf("1.9 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.9 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 4) { correct = 0; cprintf("1.9 Wrong allocation: pages are not loaded successfully into memory\n"); }
=======
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 13*Mega + 8*kilo)) { correct = 0; cprintf("Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 4) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}

	uint32 allocatedSpace = (13*Mega + 24*kilo + (INITIAL_KHEAP_ALLOCATIONS));
	uint32 allPAs[allocatedSpace/PAGE_SIZE] ;
	int numOfFrames = allocatedSpace/PAGE_SIZE ;

	//test kheap_virtual_address after kmalloc only [20%]
<<<<<<< HEAD
	cprintf("\n2. [PAGE ALLOCATOR] test kheap_virtual_address after kmalloc only [20%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		uint32 va;
		uint32 endVA = ACTUAL_START + 13*Mega + 24*kilo;
		uint32 startVA = da_limit + PAGE_SIZE;
		int i = 0;
		int j;
		for (va = startVA; va < endVA; )
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
<<<<<<< HEAD
			{ correct = 0; panic("2.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
=======
			{ correct = 0; panic("one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

			for (j = PTX(va); i < numOfFrames && j < 1024 && va < endVA; ++j, ++i)
			{
				uint32 offset = j;
				allPAs[i] = (ptr_table[j] & 0xFFFFF000) + offset;
				uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
				//cprintf("va to check = %x\n", va);
				if (retrievedVA != (va+offset))
				{
					if (correct)
					{
						cprintf("\nretrievedVA = %x, Actual VA = %x, table entry = %x, khep_pa = %x\n",retrievedVA, va + offset /*+ j*PAGE_SIZE*/, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
<<<<<<< HEAD
						correct = 0; cprintf("2.2 Wrong kheap_virtual_address\n");
=======
						correct = 0; cprintf("Wrong kheap_virtual_address\n");
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
					}
				}
				va+=PAGE_SIZE;
			}
		}
	}
	if (correct)	eval+=20 ;

	correct = 1 ;
	//kfree some of the allocated spaces
<<<<<<< HEAD
	cprintf("\n3. kfree some of the allocated spaces\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		//kfree 1st 2 MB
		int freeFrames = sys_calculate_free_frames() ;
		int freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
<<<<<<< HEAD
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("3.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512 ) { correct = 0; cprintf("3.1 Wrong kfree: pages in memory are not freed correctly\n"); }
=======
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512 ) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//kfree 2nd 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
<<<<<<< HEAD
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("3.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("3.2 Wrong kfree: pages in memory are not freed correctly\n"); }
=======
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

		//kfree 6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[7]);
<<<<<<< HEAD
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("3.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 6*Mega/4096) { correct = 0; cprintf("3.3 Wrong kfree: pages in memory are not freed correctly\n"); }
=======
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 6*Mega/4096) { correct = 0; cprintf("Wrong kfree: pages in memory are not freed correctly\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	}


	//test kheap_virtual_address after kmalloc and kfree [20%]
<<<<<<< HEAD
	cprintf("\n4. [PAGE ALLOCATOR] test kheap_virtual_address after kmalloc and kfree [20%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		uint32 va;
		uint32 endVA = ACTUAL_START + 13*Mega + 24*kilo;
		uint32 startVA = da_limit + PAGE_SIZE;
		int i = 0;
		int j;
		//frames of first 4 MB
		uint32 startIndex = (INITIAL_KHEAP_ALLOCATIONS) / PAGE_SIZE;
		for (i = startIndex ; i < startIndex + 4*Mega/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != 0)
			{
				if (correct)
<<<<<<< HEAD
				{ correct = 0; cprintf("4.1 Wrong kheap_virtual_address\n"); }
=======
				{ correct = 0; cprintf("Wrong kheap_virtual_address\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			}

		}
		//next frames until 6 MB
		for (i = startIndex + 4*Mega/PAGE_SIZE; i < startIndex + (7*Mega + 8*kilo)/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != ((startVA + i*PAGE_SIZE) + (allPAs[i] & 0xFFF)))
			{
				if (correct)
<<<<<<< HEAD
				{ correct = 0; cprintf("4.2 Wrong kheap_virtual_address\n"); }
=======
				{ correct = 0; cprintf("Wrong kheap_virtual_address\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			}
		}
		//frames of 6 MB
		for (i = startIndex + (7*Mega + 8*kilo)/PAGE_SIZE; i < startIndex + (13*Mega + 8*kilo)/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != 0)
			{
				if (correct)
<<<<<<< HEAD
				{ correct = 0; cprintf("4.3 Wrong kheap_virtual_address\n"); }
=======
				{ correct = 0; cprintf("Wrong kheap_virtual_address\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			}
		}
		//frames of last allocation (14 KB)
		for (i = startIndex + (13*Mega + 8*kilo)/PAGE_SIZE; i < startIndex + (13*Mega + 24*kilo)/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != ((startVA + i*PAGE_SIZE) + (allPAs[i] & 0xFFF)))
			{
				if (correct)
<<<<<<< HEAD
				{ correct = 0; cprintf("4.4 Wrong kheap_virtual_address\n"); }
=======
				{ correct = 0; cprintf("Wrong kheap_virtual_address\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			}
		}
	}
	if (correct)	eval+=20 ;

	correct = 1 ;
	//[DYNAMIC ALLOCATOR] test kheap_virtual_address each address [40%]
<<<<<<< HEAD
	cprintf("\n5. [DYNAMIC ALLOCATOR] test kheap_virtual_address each address [40%]\n");
	{
		uint32 va, pa;
		if ((uint32)sbrk(0) <= KERNEL_HEAP_START + PAGE_SIZE) panic("unexpected sbrk value");
=======
	{
		uint32 va, pa;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		for (va = KERNEL_HEAP_START; va < (uint32)sbrk(0); va++)
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
<<<<<<< HEAD
			{ correct = 0; panic("5.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
=======
			{ correct = 0; panic("one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			pa = (ptr_table[PTX(va)] & 0xFFFFF000) + (va & 0xFFF);
			uint32 retrievedVA = kheap_virtual_address(pa);
			if (retrievedVA != va)
			{
				if (correct)
				{
					cprintf("\nPA = %x, retrievedVA = %x expectedVA = %x\n", pa, retrievedVA, va);
<<<<<<< HEAD
					correct = 0; cprintf("5.2 Wrong kheap_virtual_address\n");
=======
					correct = 0; cprintf("Wrong kheap_virtual_address\n");
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
				}
			}
		}
	}
	if (correct)	eval+=40 ;

	correct = 1 ;
	//test kheap_virtual_address on frames of KERNEL CODE [20%]
<<<<<<< HEAD
	cprintf("\n6. test kheap_virtual_address on frames of KERNEL CODE [20%]\n");
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	{
		uint32 i;
		for (i = 1*Mega; i < (uint32)(end_of_kernel - KERNEL_BASE); i+=PAGE_SIZE)
		{
			uint32 retrievedVA = kheap_virtual_address(i);
			if (retrievedVA != 0)
			{
				if (correct)
				{
					cprintf("\nPA = %x, retrievedVA = %x\n", i, retrievedVA);
<<<<<<< HEAD
					correct = 0; cprintf("6.1 Wrong kheap_virtual_address\n");
=======
					correct = 0; cprintf("Wrong kheap_virtual_address\n");
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
				}
			}
		}
	}
	if (correct)	eval+=20 ;

	cprintf("\ntest kheap_virtual_address completed. Eval = %d%\n", eval);

	return 1;

}

<<<<<<< HEAD
// 2024
int test_ksbrk()
{
	int i, freeFrames, freeDiskFrames;
	char *ptr;
	void *ptr_allocations[20] = {0};
	uint32 actualSize;
	const int sizeOfMetaData = 8;
	int eval = 0;
	bool correct = 1;
	void* actualStart = (void*)KERNEL_HEAP_START + sizeof(int) /*BEG Block*/ + INITIAL_BLOCK_ALLOCATIONS;
	uint32* ENDBlk ;
	void* expectedVA, *expectedSBRK;
	uint32 expectedSize;
	uint32 oldBrk, newBrk;
	cprintf("\nSTEP A: check calling sbrk() in alloc_FF() with last block is allocated [25%]\n\n");
	{
		//2 KB
		actualSize = 2*kilo;
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(actualSize - sizeOfMetaData);
		expectedVA = actualStart + sizeOfMetaData/2 /*header*/;
		if (check_block(ptr_allocations[0], expectedVA, actualSize, 1) == 0)
		{ correct = 0; cprintf("A.1: Wrong block data\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0)
		{ correct = 0; cprintf("A.2: Wrong memory allocation.\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{ correct = 0; cprintf("A.3: Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }

		//=> Fill 1st page
		actualSize = PAGE_SIZE - (2*kilo + 2*sizeof(int)) - INITIAL_BLOCK_ALLOCATIONS;
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(actualSize - sizeOfMetaData);
		expectedVA = actualStart + 2*kilo + sizeOfMetaData/2 /*header*/;
		if (check_block(ptr_allocations[1], expectedVA, actualSize, 1) == 0)
		{ correct = 0; cprintf("A.4: Wrong block data\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0)
		{ correct = 0; cprintf("A.5: Wrong memory allocation.\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{ correct = 0; cprintf("A.6: Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }

		//2 KB => sbrk is called while the last block is allocated
		{
			actualSize = 2*kilo;
			freeFrames = (int)sys_calculate_free_frames() ;
			freeDiskFrames = (int)pf_calculate_free_frames() ;
			ptr_allocations[2] = kmalloc(actualSize - sizeOfMetaData);
			//check allocated block
			expectedVA = ((void*)KERNEL_HEAP_START + PAGE_SIZE - sizeof(int)) + sizeOfMetaData/2 /*header*/;
			if (check_block(ptr_allocations[2], expectedVA, actualSize, 1) == 0)
			{ correct = 0; cprintf("A.7: Wrong block data\n"); }
			//check splitted free block
			expectedVA = expectedVA + actualSize ;
			expectedSize = PAGE_SIZE - actualSize;
			if (check_block(expectedVA, expectedVA, expectedSize, 0) == 0)
			{ correct = 0; cprintf("A.8: Wrong block data\n"); }
			//check END block & BREAK
			ENDBlk = (void*)KERNEL_HEAP_START + 2*PAGE_SIZE - sizeof(int);
			if (*ENDBlk != 0x1)
			{ correct = 0; cprintf("A.9: Wrong END block after calling sbrk()\n"); }
			newBrk = (uint32)sbrk(0);
			expectedSBRK = (void*)KERNEL_HEAP_START + 2*PAGE_SIZE ;
			if ((void*)newBrk != expectedSBRK)
			{correct = 0; cprintf("A.10: Wrong new break: Expected: %x, Actual: %x\n", expectedSBRK, newBrk);}
			//check allocations in RAM & PAGE FILE
			if ((freeFrames - (int)sys_calculate_free_frames()) != 1)
			{ correct = 0; cprintf("A.11: Wrong memory allocation.\n"); }
			if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
			{ correct = 0; cprintf("A.12: Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		}

		//=> Fill 2nd page
		actualSize = PAGE_SIZE - 2*kilo ;
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[3] = kmalloc(actualSize - sizeOfMetaData);
		expectedVA = (void*)KERNEL_HEAP_START + PAGE_SIZE + 2*kilo /*include its header*/;
		if (check_block(ptr_allocations[3], expectedVA, actualSize, 1) == 0)
		{ correct = 0; cprintf("A.13: Wrong block data\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0)
		{ correct = 0; cprintf("A.14: Wrong memory allocation.\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{ correct = 0; cprintf("A.15: Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }

	}
	if (correct)
		eval += 25;

	cprintf("\nSTEP B: check calling sbrk() in alloc_FF() with last block is free (coalesce is expected) [25%]\n\n");
	{
		//2 KB: sbrk is called with last block is allocated
		oldBrk = (uint32)sbrk(0);
		actualSize = 2*kilo;
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(actualSize - sizeOfMetaData);
		expectedVA = (void*)(oldBrk - sizeof(int)) + sizeOfMetaData/2 /*header*/;
		if (check_block(ptr_allocations[0], expectedVA, actualSize, 1) == 0)
		{ correct = 0; cprintf("B.1: Wrong block data\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 1)
		{ correct = 0; cprintf("B.2: Wrong memory allocation.\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{ correct = 0; cprintf("B.3: Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }

		//1 KB
		actualSize = 1*kilo;
		freeFrames = (int)sys_calculate_free_frames() ;
		freeDiskFrames = (int)pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(actualSize - sizeOfMetaData);
		expectedVA = expectedVA + 2*kilo ;
		if (check_block(ptr_allocations[1], expectedVA, actualSize, 1) == 0)
		{ correct = 0; cprintf("B.4: Wrong block data\n"); }
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0)
		{ correct = 0; cprintf("B.5: Wrong memory allocation.\n"); }
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{ correct = 0; cprintf("B.6: Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }

		//2 KB => sbrk is called while the last block is free
		{
			actualSize = 2*kilo;
			freeFrames = (int)sys_calculate_free_frames() ;
			freeDiskFrames = (int)pf_calculate_free_frames() ;
			ptr_allocations[2] = kmalloc(actualSize - sizeOfMetaData);
			//check allocated block
			expectedVA = expectedVA + 1*kilo ;
			if (check_block(ptr_allocations[2], expectedVA, actualSize, 1) == 0)
			{ correct = 0; cprintf("B.7: Wrong block data\n"); }
			//check splitted free block
			expectedVA = expectedVA + actualSize ;
			expectedSize = PAGE_SIZE - (1*kilo - sizeof(int)) - sizeof(int) /*END block*/;
			if (check_block(expectedVA, expectedVA, expectedSize, 0) == 0)
			{ correct = 0; cprintf("B.8: Wrong block data\n"); }
			//check END block & BREAK
			ENDBlk = (void*)oldBrk + 2*PAGE_SIZE - sizeof(int);
			if (*ENDBlk != 0x1)
			{ correct = 0; cprintf("B.9: Wrong END block after calling sbrk()\n"); }
			newBrk = (uint32)sbrk(0);
			expectedSBRK = (void*)oldBrk + 2*PAGE_SIZE ;
			if ((void*)newBrk != expectedSBRK)
			{correct = 0; cprintf("B.10: Wrong new break: Expected: %x, Actual: %x\n", expectedSBRK, newBrk);}
			//check allocations in RAM & PAGE FILE
			if ((freeFrames - (int)sys_calculate_free_frames()) != 1)
			{ correct = 0; cprintf("B.11: Wrong memory allocation.\n"); }
			if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
			{ correct = 0; cprintf("B.12: Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		}
	}
	if (correct)
		eval += 25;

	uint32 numOfCases = 5;
	uint32 incNumOfPages[] = {0, 1, 2, 11, DYN_ALLOC_MAX_SIZE/PAGE_SIZE - 13};
	uint32 expectedVAs[] = {
			KERNEL_HEAP_START + 0x1000, // 0
			KERNEL_HEAP_START + 0x1000, // 1
			KERNEL_HEAP_START + 0x2000, // 2
			KERNEL_HEAP_START + 0x4000, // 11
			0xFFFFFFFF, 				// exceed (RETURN -1)
	};
	uint32 expectedSbrks[] = {
			KERNEL_HEAP_START + 0x1000, // 0
			KERNEL_HEAP_START + 0x2000, // 1
			KERNEL_HEAP_START + 0x4000, // 2
			KERNEL_HEAP_START + 0xF000, // 11
			KERNEL_HEAP_START + 0xF000, // exceed
	};
	//Reinitialize the kernel block allocator
	initialize_kheap_dynamic_allocator(KERNEL_HEAP_START, PAGE_SIZE, KERNEL_HEAP_START + DYN_ALLOC_MAX_SIZE);
	for (uint32 va = KERNEL_HEAP_START+PAGE_SIZE; va < KERNEL_HEAP_START + DYN_ALLOC_MAX_SIZE; va += PAGE_SIZE)
	{
		unmap_frame(ptr_page_directory, va);
	}
	cprintf("\nSTEP C: checking sbrk() increment with zero & +ve values [40%]\n\n");
	{
		for (int i = 0; i < numOfCases - 1; ++i)
		{
			freeFrames = (int)sys_calculate_free_frames();
			freeDiskFrames = (int)pf_calculate_free_frames();
			oldBrk = (uint32)sbrk(0);
			void* VA = sbrk(incNumOfPages[i]);
			newBrk = (uint32)sbrk(0);
			correct = 1;
			if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
			{
				correct = 0;
				cprintf("C.%d: Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n", i);
			}
			if ((freeFrames - (int)sys_calculate_free_frames()) != incNumOfPages[i])
			{
				correct = 0;
				cprintf("C.%d: Wrong memory allocation\n", i);
			}
			if ((uint32)VA != expectedVAs[i])
			{
				correct = 0;
				cprintf("C.%d: Wrong returned break: Expected: %x, Actual: %x\n", i, expectedVAs[i], VA);
			}
			if (newBrk != expectedSbrks[i])
			{
				correct = 0;
				cprintf("C.%d: Wrong new break: Expected: %x, Actual: %x\n", i, expectedSbrks[i], newBrk);
			}
			if (correct)
				eval += 10;
		}
	}
	cprintf("\nSTEP D: checking sbrk() increment with LARGE +ve value (EXCEED LIMIT) [10%]\n\n");
	{
		for (int i = numOfCases - 1; i < numOfCases ; ++i)
		{
			freeFrames = (int)sys_calculate_free_frames();
			freeDiskFrames = (int)pf_calculate_free_frames();
			oldBrk = (uint32)sbrk(0);
			void* VA = sbrk(incNumOfPages[i]);
			newBrk = (uint32)sbrk(0);
			correct = 1;
			if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
			{
				correct = 0;
				cprintf("D.%d: Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n", i);
			}
			if ((freeFrames - (int)sys_calculate_free_frames()) != 0)
			{
				correct = 0;
				cprintf("D.%d: Wrong memory allocation\n", i);
			}
			if ((uint32)VA != expectedVAs[i])
			{
				correct = 0;
				cprintf("D.%d: Wrong returned break: Expected: %x, Actual: %x\n", i, expectedVAs[i], VA);
			}
			if (newBrk != expectedSbrks[i])
			{
				correct = 0;
				cprintf("D.%d: Wrong new break: Expected: %x, Actual: %x\n", i, expectedSbrks[i], newBrk);
			}
			if (correct)
				eval += 10;
		}
	}

	cprintf("\nTest kheap sbrk completed. Eval = %d%%\n\n", eval);
=======

// 2023
int test_ksbrk()
{
	// char minByte = 1 << 7;
	// char maxByte = 0x7F;
	// short minShort = 1 << 15;
	// short maxShort = 0x7FFF;
	// int minInt = 1 << 31;
	// int maxInt = 0x7FFFFFFF;

	// char *byteArr, *byteArr2;
	// short *shortArr, *shortArr2;
	// int *intArr;
	// struct MyStruct *structArr;
	// int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	// int start_freeFrames = (int)sys_calculate_free_frames();

	// malloc some spaces
	int i, freeFrames, freeDiskFrames;
	char *ptr;
	// int lastIndices[20] = {0};
	int sums[20] = {0};
	void *ptr_allocations[20] = {0};

	// uint32 inputIncrementValues[] = {0, kilo, 2*kilo, -512, -2 * kilo, -2* kilo, 128, kilo};
	uint32 expectedVAs[] = {
			KERNEL_HEAP_START + 0x1000, // 0
			KERNEL_HEAP_START + 0x1000, // kilo
			KERNEL_HEAP_START + 0x2000, // 2*kilo
			KERNEL_HEAP_START + 0x2E00, // -512
			KERNEL_HEAP_START + 0x2600, // -2*kilo
			KERNEL_HEAP_START + 0x1E00, // -2*kilo
			KERNEL_HEAP_START + 0x1E00, // 128
			KERNEL_HEAP_START + 0x2000, // kilo
			KERNEL_HEAP_START + 0x0C00, // -9*kilo
			KERNEL_HEAP_START + 0x0C00, // +6*kilo
	};
	uint32 expectedSbrks[] = {
			KERNEL_HEAP_START + 0x1000, // 0
			KERNEL_HEAP_START + 0x2000, // kilo
			KERNEL_HEAP_START + 0x3000, // 2*kilo
			KERNEL_HEAP_START + 0x2E00, // -512
			KERNEL_HEAP_START + 0x2600, // -2*kilo
			KERNEL_HEAP_START + 0x1E00, // -2*kilo
			KERNEL_HEAP_START + 0x2000, // 128
			KERNEL_HEAP_START + 0x3000, // kilo
			KERNEL_HEAP_START + 0x0C00, // -9*kilo
			KERNEL_HEAP_START + 0x4000, // +10*kilo
	};
	uint32 oldBrk, newBrk;
	int eval = 0;
	bool correct = 1;

	cprintf("STEP A: checking increment with ZERO\n");
	{
		freeFrames = (int)sys_calculate_free_frames();
		freeDiskFrames = (int)pf_calculate_free_frames();
		ptr_allocations[0] = sbrk(0);
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{
			correct = 0;
			cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		}
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0)
		{
			correct = 0;
			cprintf("Wrong allocation: pages are not loaded successfully into memory");
		}
		if ((uint32)ptr_allocations[0] != expectedVAs[0])
		{
			correct = 0;
			cprintf("Wrong returned break: Expected: %x, Actual: %x\n", expectedVAs[0], ptr_allocations[0]);
		}
		if (correct)
			eval += 5;
	}
	cprintf("STEP B: checking increment with +ve value\n");
	{ // +1 KB
		freeFrames = (int)sys_calculate_free_frames();
		freeDiskFrames = (int)pf_calculate_free_frames();
		oldBrk = (uint32)sbrk(0);
		ptr_allocations[1] = sbrk(kilo);
		newBrk = (uint32)sbrk(0);
		correct = 1;
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{
			correct = 0;
			cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		}
		// cprintf("####### %x - %x\n", freeFrames - (int)sys_calculate_free_frames(), -1 * ((ROUNDUP(oldBrk, PAGE_SIZE) - newBrk) / PAGE_SIZE));
		if ((freeFrames - (int)sys_calculate_free_frames()) != 1)
		{
			correct = 0;
			cprintf("Wrong allocation: pages are not loaded successfully into memory");
		}
		if ((uint32)ptr_allocations[1] != expectedVAs[1])
		{
			correct = 0;
			cprintf("Wrong returned break: Expected: %x, Actual: %x\n", expectedVAs[1], ptr_allocations[1]);
		}
		if (newBrk != expectedSbrks[1])
		{
			correct = 0;
			cprintf("Wrong new break: Expected: %x, Actual: %x\n", newBrk, expectedSbrks[1]);
		}
		if (correct)
			eval += 5;
	}
	{ // +2 KB
		freeFrames = (int)sys_calculate_free_frames();
		freeDiskFrames = (int)pf_calculate_free_frames();
		oldBrk = (uint32)sbrk(0);
		ptr_allocations[2] = sbrk(2 * kilo);
		newBrk = (uint32)sbrk(0);
		correct = 1;
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{
			correct = 0;
			cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		}
		int x = (freeFrames - (int)sys_calculate_free_frames());
		if ((freeFrames - (int)sys_calculate_free_frames()) != 1)
		{
			correct = 0;
			cprintf("Wrong allocation: pages are not loaded successfully into memory");
		}
		if ((uint32)ptr_allocations[2] != expectedVAs[2])
		{
			correct = 0;
			cprintf("Wrong returned break: Expected: %x, Actual: %x\n", expectedVAs[2], ptr_allocations[2]);
		}
		if (newBrk != expectedSbrks[2])
		{
			correct = 0;
			cprintf("Wrong new break: Expected: %x, Actual: %x\n", newBrk, expectedSbrks[2]);
		}
		if (correct)
			eval += 5;
	}
	cprintf("STEP C: checking increment with -ve value [No Frames to be Deallocated]\n");
	{ // -512 Bytes
		freeFrames = (int)sys_calculate_free_frames();
		freeDiskFrames = (int)pf_calculate_free_frames();
		oldBrk = (uint32)sbrk(0);
		ptr_allocations[3] = sbrk(-512);
		newBrk = (uint32)sbrk(0);
		correct = 1;
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{
			correct = 0;
			cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		}
		// cprintf("####### %x - %x\n", freeFrames - (int)sys_calculate_free_frames(), -1 * ((ROUNDUP(oldBrk, PAGE_SIZE) - newBrk) / PAGE_SIZE));
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0)
		{
			correct = 0;
			cprintf("Wrong allocation: pages are not loaded successfully into memory");
		}
		if ((uint32)ptr_allocations[3] != expectedVAs[3])
		{
			correct = 0;
			cprintf("Wrong returned break: Expected: %x, Actual: %x\n", expectedVAs[3], ptr_allocations[3]);
		}
		if (newBrk != expectedSbrks[3])
		{
			correct = 0;
			cprintf("Wrong new break: Expected: %x, Actual: %x\n", newBrk, expectedSbrks[3]);
		}
		if (correct)
			eval += 10;
	}
	{ // -2 KB
		freeFrames = (int)sys_calculate_free_frames();
		freeDiskFrames = (int)pf_calculate_free_frames();
		oldBrk = (uint32)sbrk(0);
		ptr_allocations[4] = sbrk(-2 * kilo);
		newBrk = (uint32)sbrk(0);
		correct = 1;
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{
			correct = 0;
			cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		}
		// cprintf("####### %x - %x\n", freeFrames - (int)sys_calculate_free_frames(), -1 * ((ROUNDUP(oldBrk, PAGE_SIZE) - newBrk) / PAGE_SIZE));
		if ((freeFrames - (int)sys_calculate_free_frames()) != 0)
		{
			correct = 0;
			cprintf("Wrong allocation: pages are not loaded successfully into memory");
		}
		if ((uint32)ptr_allocations[4] != expectedVAs[4])
		{
			correct = 0;
			cprintf("Wrong returned break: Expected: %x, Actual: %x\n", expectedVAs[4], ptr_allocations[4]);
		}
		if (newBrk != expectedSbrks[4])
		{
			correct = 0;
			cprintf("Wrong new break: Expected: %x, Actual: %x\n", newBrk, expectedSbrks[4]);
		}
		if (correct)
			eval += 10;
	}
	cprintf("STEP D: checking increment with -ve value [ONE Frame should be Deallocated]\n");
	{ // -2 KB
		freeFrames = (int)sys_calculate_free_frames();
		freeDiskFrames = (int)pf_calculate_free_frames();
		oldBrk = (uint32)sbrk(0);
		ptr_allocations[5] = sbrk(-2 * kilo);
		newBrk = (uint32)sbrk(0);
		correct = 1;
		if (((int)(int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{
			correct = 0;
			cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		}
		// cprintf("####### %x - %x\n", freeFrames - (int)sys_calculate_free_frames(), -1 * ((ROUNDUP(oldBrk, PAGE_SIZE) - newBrk) / PAGE_SIZE));
		if (((int)(int)sys_calculate_free_frames() - freeFrames) != 1)
		{
			correct = 0;
			cprintf("Wrong allocation: pages are not loaded successfully into memory");
		}
		if ((uint32)ptr_allocations[5] != expectedVAs[5])
		{
			correct = 0;
			cprintf("Wrong returned break: Expected: %x, Actual: %x\n", expectedVAs[5], ptr_allocations[5]);
		}
		if (newBrk != expectedSbrks[5])
		{
			correct = 0;
			cprintf("Wrong new break: Expected: %x, Actual: %x\n", newBrk, expectedSbrks[5]);
		}
		if (correct)
			eval += 15;
	}
	cprintf("STEP E: checking increment with +ve value [No Frames to be Allocated]\n");
	{ // 128 Bytes
		freeFrames = (int)(int)sys_calculate_free_frames();
		freeDiskFrames = (int)(int)pf_calculate_free_frames();
		oldBrk = (uint32)sbrk(0);
		ptr_allocations[6] = sbrk(128);
		newBrk = (uint32)sbrk(0);
		correct = 1;
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{
			correct = 0;
			cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		}
		if (((int)(int)sys_calculate_free_frames() - freeFrames) != 0)
		{
			correct = 0;
			cprintf("Wrong allocation: pages are not loaded successfully into memory");
		}
		if ((uint32)ptr_allocations[6] != expectedVAs[6])
		{
			correct = 0;
			cprintf("Wrong returned break: Expected: %x, Actual: %x\n", expectedVAs[6], ptr_allocations[6]);
		}
		if (newBrk != expectedSbrks[6])
		{
			correct = 0;
			cprintf("Wrong new break: Expected: %x, Actual: %x\n", newBrk, expectedSbrks[6]);
		}
		if (correct)
			eval += 15;
	}
	cprintf("STEP F: checking increment with +ve value [ONE Frame should be Allocated]\n");
	{ // 1 KB
		freeFrames = (int)(int)sys_calculate_free_frames();
		freeDiskFrames = (int)pf_calculate_free_frames();
		oldBrk = (uint32)sbrk(0);
		ptr_allocations[7] = sbrk(kilo);
		newBrk = (uint32)sbrk(0);
		correct = 1;
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{
			correct = 0;
			cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		}
		//cprintf("((int)(int)sys_calculate_free_frames() - freeFrames) = %d\n", ((int)(int)sys_calculate_free_frames() - freeFrames));
		if ((freeFrames - (int)(int)sys_calculate_free_frames()) != 1)
		{
			correct = 0;
			cprintf("7 Wrong allocation: pages are not loaded successfully into memory");
		}
		if ((uint32)ptr_allocations[7] != expectedVAs[7])
		{
			correct = 0;
			cprintf("7 Wrong returned break: Expected: %x, Actual: %x\n", expectedVAs[7], ptr_allocations[7]);
		}
		if (newBrk != expectedSbrks[7])
		{
			correct = 0;
			cprintf("7 Wrong new break: Expected: %x, Actual: %x\n", newBrk, expectedSbrks[7]);
		}
		if (correct)
			eval += 15;
	}
	cprintf("STEP G: checking increment with -ve value [TWO Frames should be Deallocated]\n");
	{ // -9 KB
		freeFrames = (int)sys_calculate_free_frames();
		freeDiskFrames = (int)pf_calculate_free_frames();
		oldBrk = (uint32)sbrk(0);
		ptr_allocations[8] = sbrk(-9 * kilo);
		newBrk = (uint32)sbrk(0);
		correct = 1;
		if (((int)(int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{
			correct = 0;
			cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		}
		// cprintf("####### %x - %x\n", freeFrames - (int)sys_calculate_free_frames(), -1 * ((ROUNDUP(oldBrk, PAGE_SIZE) - newBrk) / PAGE_SIZE));
		if (((int)(int)sys_calculate_free_frames() - freeFrames) != 2)
		{
			correct = 0;
			cprintf("8 Wrong allocation: pages are not loaded successfully into memory");
		}
		if ((uint32)ptr_allocations[8] != expectedVAs[8])
		{
			correct = 0;
			cprintf("8 Wrong returned break: Expected: %x, Actual: %x\n", expectedVAs[5], ptr_allocations[5]);
		}
		if (newBrk != expectedSbrks[8])
		{
			correct = 0;
			cprintf("8 Wrong new break: Expected: %x, Actual: %x\n", newBrk, expectedSbrks[5]);
		}
		if (correct)
			eval += 10;
	}
	cprintf("STEP H: checking increment with +ve value [THREE Frames should be Allocated]\n");
	{ // 10 KB
		freeFrames = (int)(int)sys_calculate_free_frames();
		freeDiskFrames = (int)pf_calculate_free_frames();
		oldBrk = (uint32)sbrk(0);
		ptr_allocations[9] = sbrk(10*kilo);
		newBrk = (uint32)sbrk(0);
		correct = 1;
		if (((int)pf_calculate_free_frames() - freeDiskFrames) != 0)
		{
			correct = 0;
			cprintf("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		}
		if ((freeFrames - (int)(int)sys_calculate_free_frames()) != 3)
		{
			correct = 0;
			cprintf("9 Wrong allocation: pages are not loaded successfully into memory");
		}
		if ((uint32)ptr_allocations[9] != expectedVAs[9])
		{
			correct = 0;
			cprintf("9 Wrong returned break: Expected: %x, Actual: %x\n", expectedVAs[7], ptr_allocations[7]);
		}
		if (newBrk != expectedSbrks[9])
		{
			correct = 0;
			cprintf("9 Wrong new break: Expected: %x, Actual: %x\n", newBrk, expectedSbrks[7]);
		}
		if (correct)
			eval += 10;
	}

	//cprintf("Test kheap sbrk completed. Evaluation = %d%%\n", eval);
	cprintf("[AUTO_GR@DING_PARTIAL]%d\n", eval);
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

	cprintf("=================\n\n");
	return 0;
}


<<<<<<< HEAD
//==============================================================================================//
//==============================================================================================//
//==============================================================================================//
//==============================================================================================//

int test_kmalloc_nextfit()
{
	panic("not implemented");
=======















int test_kmalloc_nextfit()
{
	panic("not handled yet after applying dynamic allocator with page allocator");

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	void* ptr_allocations[160] = {0};
	cprintf("This test has THREE cases. A pass message will be displayed after each one.\n");

	// allocate pages
	int freeFrames = sys_calculate_free_frames() ;
	int freeDiskFrames = pf_calculate_free_frames() ;

	int i;
	//ptr_allocations[0] = kmalloc(2*Mega - KERNEL_SHARES_ARR_INIT_SIZE - KERNEL_SEMAPHORES_ARR_INIT_SIZE);
	for(i = 0; i< 79 ;i++)
	{
		ptr_allocations[i] = kmalloc(2*Mega);
	}
	ptr_allocations[79] = kmalloc(2*Mega - PAGE_SIZE - INITIAL_KHEAP_ALLOCATIONS);


	// randomly check the addresses of the allocation
	if( 	(uint32)ptr_allocations[0] != ACTUAL_START ||
			(uint32)ptr_allocations[2] != (ACTUAL_START + 4*Mega) ||
			(uint32)ptr_allocations[8] != (ACTUAL_START + 16*Mega) ||
			(uint32)ptr_allocations[10] != (ACTUAL_START + 20*Mega) ||
			(uint32)ptr_allocations[15] != (ACTUAL_START + 30*Mega) ||
			(uint32)ptr_allocations[20] != (ACTUAL_START + 40*Mega) ||
			(uint32)ptr_allocations[25] != (ACTUAL_START + 50*Mega) ||
			(uint32)ptr_allocations[79] != (ACTUAL_START + 158*Mega ))
		panic("Wrong allocation, Check next fitting strategy is working correctly");

	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) != (160*Mega - PAGE_SIZE - INITIAL_KHEAP_ALLOCATIONS)/(PAGE_SIZE) ) panic("Wrong allocation");

	// Make memory holes.
	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;

	kfree(ptr_allocations[0]);		// Hole 1 = 2 M
	kfree(ptr_allocations[2]);		// Hole 2 = 4 M
	kfree(ptr_allocations[3]);
	kfree(ptr_allocations[5]);		// Hole 3 = 2 M
	kfree(ptr_allocations[10]);		// Hole 4 = 6 M
	kfree(ptr_allocations[12]);
	kfree(ptr_allocations[11]);
	kfree(ptr_allocations[20]);		// Hole 5 = 2 M
	kfree(ptr_allocations[25]);		// Hole 6 = 2 M
	kfree(ptr_allocations[79]);		// Hole 7 = 2 M - 4 KB

	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((sys_calculate_free_frames() - freeFrames) != ((10*2*Mega) - PAGE_SIZE - INITIAL_KHEAP_ALLOCATIONS)/PAGE_SIZE) panic("Wrong free: Extra or less pages are removed from main memory");

	// Test next fit
	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	void* tempAddress = kmalloc(Mega-kilo);		// Use Hole 1 -> Hole 1 = 1 M
	if((uint32)tempAddress != ACTUAL_START)
		panic("Next Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) != (1*Mega)/PAGE_SIZE) panic("Wrong allocation");

	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	tempAddress = kmalloc(kilo);					// Use Hole 1 -> Hole 1 = 1 M - Kilo -> requires one page only
	if((uint32)tempAddress != ACTUAL_START + 0x00100000)
		panic("Next Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation");

	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	tempAddress = kmalloc(5*Mega); 			   // Use Hole 4 -> Hole 4 = 1 M
	if((uint32)tempAddress != ACTUAL_START + 0x01400000)
		panic("Next Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) != (5*Mega)/PAGE_SIZE) panic("Wrong allocation");

	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	tempAddress = kmalloc(1*Mega); 			   // Use Hole 4 -> Hole 4 = 0 M
	if((uint32)tempAddress != ACTUAL_START + 0x01900000)
		panic("Next Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) != (1*Mega)/PAGE_SIZE) panic("Wrong allocation");

	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	kfree(ptr_allocations[15]);					// Make a new hole => 2 M
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((sys_calculate_free_frames() - freeFrames) !=  (2*Mega)/PAGE_SIZE) panic("Wrong free: Extra or less pages are removed from main memory");

	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	tempAddress = kmalloc(kilo); 			   // Use new Hole = 2 M - 4 kilo
	if((uint32)tempAddress != ACTUAL_START + 0x01E00000)
		panic("Next Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation");

	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	tempAddress = kmalloc(Mega + 1016*kilo); 	// Use new Hole = 4 kilo
	if((uint32)tempAddress != ACTUAL_START + 0x01E01000)
		panic("Next Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");

	if ((freeFrames - sys_calculate_free_frames()) != (1*Mega+1016*kilo)/PAGE_SIZE) panic("Wrong allocation");

	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	tempAddress = kmalloc(512*kilo); 			   // Use Hole 5 -> Hole 5 = 1.5 M
	if((uint32)tempAddress != ACTUAL_START + 0x02800000)
		panic("Next Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) != (512*kilo)/PAGE_SIZE) panic("Wrong allocation");

	cprintf("\nCASE1: (next fit without looping back) is succeeded...\n") ;
	/******************************/

	// Check that next fit is looping back to check for free space
	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	tempAddress = kmalloc(3*Mega + 512*kilo); 			   // Use Hole 2 -> Hole 2 = 0.5 M
	if((uint32)tempAddress != ACTUAL_START + 0x00400000)
		panic("Next Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) != (3*Mega+512*kilo)/PAGE_SIZE) panic("Wrong allocation");

	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	kfree(ptr_allocations[24]);		// Increase size of Hole 6 to 4 M
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((sys_calculate_free_frames() - freeFrames) != (2*Mega)/PAGE_SIZE) panic("Wrong free: Extra or less pages are removed from main memory");

	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	tempAddress = kmalloc(4*Mega-kilo);		// Use Hole 6 -> Hole 6 = 0 M
	if((uint32)tempAddress != ACTUAL_START + 0x03000000)
		panic("Next Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) != (4*Mega)/PAGE_SIZE) panic("Wrong allocation");

	cprintf("\nCASE2: (next fit WITH looping back) is succeeded...\n") ;
	/******************************/

	// Check that next fit returns null in case all holes are not free
	freeDiskFrames = pf_calculate_free_frames() ;
	freeFrames = sys_calculate_free_frames() ;
	tempAddress = kmalloc(6*Mega); 			   // No Suitable Hole is available
	if((uint32)tempAddress != 0x0)
		panic("Next Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation");

	cprintf("\nCASE3: (next fit with insufficient space) is succeeded...\n") ;
	/******************************/

	cprintf("Congratulations!! test Next Fit completed successfully.\n");
	return 1;

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}

int test_kmalloc_bestfit1()
{
<<<<<<< HEAD
	panic("not implemented");
=======
	panic("not handled yet after applying dynamic allocator with page allocator");

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	void* ptr_allocations[20] = {0};
	uint32 freeFrames;
	uint32 freeDiskFrames;

	//[1] Allocate all
	{
		//Allocate 3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[0] != (ACTUAL_START)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != ((3*Mega)/PAGE_SIZE)) panic("Wrong allocation: ");

		//Allocate 3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[1] !=  (ACTUAL_START + 3*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != ((3*Mega)/PAGE_SIZE)) panic("Wrong allocation: ");

		//Allocate 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[2] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[2] !=  (ACTUAL_START + 6*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != ((2*Mega)/PAGE_SIZE)) panic("Wrong allocation: ");

		//Allocate 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[3] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[3] != (ACTUAL_START + 8*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  ((2*Mega)/PAGE_SIZE)) panic("Wrong allocation: ");

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[4] = kmalloc(1*Mega-kilo);
		if ((uint32) ptr_allocations[4] !=  (ACTUAL_START + 10*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong allocation: ");

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(1*Mega-kilo);
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 11*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong allocation: ");

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(1*Mega-kilo);
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 12*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong allocation: ");

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(1*Mega-kilo);
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 13*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong allocation: ");
	}

	//[2] Free some to create holes
	{
		//3 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != ((3*Mega)/PAGE_SIZE)) panic("Wrong free: ");

		//2 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[3]);
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != ((2*Mega)/PAGE_SIZE)) panic("Wrong free: ");

		//1 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[5]);
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 256) panic("Wrong free: ");
	}

	//[3] Allocate again [test best fit]
	{
		//Allocate 512 KB - should be placed in 3rd hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[8] = kmalloc(512*kilo);
		if ((uint32) ptr_allocations[8] !=  (ACTUAL_START + 11*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 128) panic("Wrong allocation: ");

		//Allocate 1 MB - should be placed in 2nd hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[9] = kmalloc(1*Mega - kilo);
		if ((uint32) ptr_allocations[9] !=  (ACTUAL_START + 8*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong allocation: ");

		//Allocate 256 KB - should be placed in remaining of 3rd hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[10] = kmalloc(256*kilo - kilo);
		if ((uint32) ptr_allocations[10] !=  (ACTUAL_START + 11*Mega + 512*kilo)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 64) panic("Wrong allocation: ");

		//Allocate 4 MB - should be placed in end of all allocations
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[11] = kmalloc(4*Mega - kilo);
		if ((uint32) ptr_allocations[11] != (ACTUAL_START + 14*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1024) panic("Wrong allocation: ");
	}

	//[4] Free contiguous allocations
	{
		//1M Hole appended to already existing 1M hole in the middle
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[4]);
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 256) panic("Wrong free: ");

		//another 512 KB Hole appended to the hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[8]);
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 128) panic("Wrong free: ");
	}

	//[5] Allocate again [test best fit]
	{
		//Allocate 2 MB - should be placed in the contiguous hole (2 MB + 512 KB)
		freeFrames = sys_calculate_free_frames();
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[12] = kmalloc(2*Mega - kilo);
		if ((uint32) ptr_allocations[12] != (ACTUAL_START + 9*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: ");
	}

	cprintf("Congratulations!! test BEST FIT allocation (1) completed successfully.\n");

	return 1;

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}

int test_kmalloc_bestfit2()
{
<<<<<<< HEAD
	panic("not implemented");
=======
	panic("not handled yet after applying dynamic allocator with page allocator");

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	void* ptr_allocations[20] = {0};
	uint32 freeFrames;
	uint32 freeDiskFrames;

	//[1] Attempt to allocate more than heap size
	{
		ptr_allocations[0] = kmalloc(KERNEL_HEAP_MAX - ACTUAL_START + 1);
		if (ptr_allocations[0] != NULL) panic("Kmalloc: Attempt to allocate more than heap size, should return NULL");
	}

	//[2] Attempt to allocate space more than any available fragment
	//	a) Create Fragments
	{
		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[0] != (ACTUAL_START)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  512) panic("Wrong allocation: ");

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  512) panic("Wrong allocation: ");

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[2] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[2] != (ACTUAL_START + 4*Mega)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  1) panic("Wrong allocation: ");

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[3] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[3] != (ACTUAL_START + 4*Mega + 4*kilo)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  1) panic("Wrong allocation: ");

		//4 KB Hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		kfree(ptr_allocations[2]);
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 1) panic("Wrong allocation: ");

		//7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[4] = kmalloc(7*kilo);
		if ((uint32) ptr_allocations[4] != (ACTUAL_START + 4*Mega + 8*kilo)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  2) panic("Wrong allocation: ");

		//2 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		kfree(ptr_allocations[0]);
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 512) panic("Wrong free: Extra or less pages are removed from main memory");

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[5] = kmalloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega + 16*kilo)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  768) panic("Wrong allocation: ");

		//2 MB + 6 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[6] = kmalloc(2*Mega + 6*kilo);
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 7*Mega + 16*kilo)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  514) panic("Wrong allocation: ");

		//5 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[7] = kmalloc(5*Mega-kilo);
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 9*Mega + 24*kilo)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  ((5*Mega)/PAGE_SIZE)) panic("Wrong allocation: ");

		//2 MB + 8 KB Hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		kfree(ptr_allocations[6]);
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) !=  514) panic("Wrong free: Extra or less pages are removed from main memory");

		//2 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		kfree(ptr_allocations[1]);
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) !=  512) panic("Wrong free: Extra or less pages are removed from main memory.");

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[8] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 7*Mega + 16*kilo)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  512) panic("Wrong allocation:");

		//6 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[9] = kmalloc(6*kilo);
		if ((uint32) ptr_allocations[9] != (ACTUAL_START + 9*Mega + 16*kilo)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  2) panic("Wrong allocation:");

		//3 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		kfree(ptr_allocations[5]);
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) !=  768) panic("Wrong free: Extra or less pages are removed from main memory.");

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[10] = kmalloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[10] != (ACTUAL_START + 4*Mega + 16*kilo)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) !=  ((3*Mega)/4096)) panic("Wrong free: Extra or less pages are removed from main memory.");

		//4 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames();
		ptr_allocations[11] = kmalloc(4*Mega-kilo);
		if ((uint32) ptr_allocations[11] != (ACTUAL_START)) panic("Wrong start address for the allocated space... ");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != ((4*Mega)/4096)) panic("Wrong free: Extra or less pages are removed from main memory.");

	}

	//	b) Attempt to allocate large segment with no suitable fragment to fit on
	{
		//Large Allocation
		ptr_allocations[12] = kmalloc((KERNEL_HEAP_MAX - ACTUAL_START - 14*Mega));
		if (ptr_allocations[12] != NULL) panic("Kmalloc: Attempt to allocate large segment with no suitable fragment to fit on, should return NULL");

		cprintf("Congratulations!! test BEST FIT allocation (2) completed successfully.\n");
	}
	return 1;

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}

int test_kmalloc_worstfit()
{
<<<<<<< HEAD
	panic("not implemented");
=======
	panic("not handled yet after applying dynamic allocator with page allocator");

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	void* ptr_allocations[160] = {0};

	// allocate pages
	int freeFrames = sys_calculate_free_frames() ;
	int freeDiskFrames = pf_calculate_free_frames() ;

	int count = 0;
	int i;
	for(i = 0; i< 79 ;i++)
	{
		ptr_allocations[i] = kmalloc(2*Mega);
	}
	ptr_allocations[79] = kmalloc(2*Mega - PAGE_SIZE /*- KERNEL_SHARES_ARR_INIT_SIZE - KERNEL_SEMAPHORES_ARR_INIT_SIZE*/);

	// randomly check the addresses of the allocation
	if( 	(uint32)ptr_allocations[0] != ACTUAL_START ||
			(uint32)ptr_allocations[2] != (ACTUAL_START + 4*Mega) ||
			(uint32)ptr_allocations[8] != (ACTUAL_START + 16*Mega) ||
			(uint32)ptr_allocations[10] != (ACTUAL_START + 20*Mega) ||
			(uint32)ptr_allocations[15] != (ACTUAL_START + 30*Mega) ||
			(uint32)ptr_allocations[20] != (ACTUAL_START + 40*Mega) ||
			(uint32)ptr_allocations[50] != (ACTUAL_START + 100*Mega) ||
			(uint32)ptr_allocations[79] != (ACTUAL_START + 158*Mega))
		panic("Wrong allocation, Check worst fitting strategy is working correctly");

	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) != (160*Mega - PAGE_SIZE /*- KERNEL_SHARES_ARR_INIT_SIZE - KERNEL_SEMAPHORES_ARR_INIT_SIZE*/)/(PAGE_SIZE) ) panic("Wrong allocation");

	//make memory holes
	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames() ;

	kfree(ptr_allocations[0]);		//Hole 1 = 2 M
	kfree(ptr_allocations[2]);		//Hole 2 = 4 M
	kfree(ptr_allocations[3]);
	kfree(ptr_allocations[10]);		//Hole 3 = 6 M
	kfree(ptr_allocations[12]);
	kfree(ptr_allocations[11]);
	kfree(ptr_allocations[30]);		//Hole 4 = 10 M
	kfree(ptr_allocations[31]);
	kfree(ptr_allocations[32]);
	kfree(ptr_allocations[33]);
	kfree(ptr_allocations[34]);
	kfree(ptr_allocations[70]); 	//Hole 5 = 8 M
	kfree(ptr_allocations[71]);
	kfree(ptr_allocations[72]);
	kfree(ptr_allocations[73]);

	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((sys_calculate_free_frames() - freeFrames) != ((15*2*Mega))/PAGE_SIZE) panic("Wrong free: Extra or less pages are removed from main memory");

	// Test worst fit
	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames();
	void* tempAddress = kmalloc(Mega);		//Use Hole 4 -> Hole 4 = 9 M
	if((uint32)tempAddress != ACTUAL_START + 0x03C00000)
		panic("Worst Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) !=  1*Mega/PAGE_SIZE) panic("Wrong allocation:");
	cprintf("Test %d Passed \n", ++count);

	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames();
	tempAddress = kmalloc(4 * Mega);			//Use Hole 4 -> Hole 4 = 5 M
	if((uint32)tempAddress != ACTUAL_START + 0x03D00000)
		panic("Worst Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) !=  4*Mega/PAGE_SIZE) panic("Wrong allocation:");
	cprintf("Test %d Passed \n", ++count);

	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames();
	tempAddress = kmalloc(6*Mega); 			   //Use Hole 5 -> Hole 5 = 2 M
	if((uint32)tempAddress != ACTUAL_START + 0x08C00000)
		panic("Worst Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) !=  6*Mega/PAGE_SIZE) panic("Wrong allocation:");
	cprintf("Test %d Passed \n", ++count);

	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames();
	tempAddress = kmalloc(5*Mega); 			   //Use Hole 3 -> Hole 3 = 1 M
	if((uint32)tempAddress != ACTUAL_START + 0x01400000)
		panic("Worst Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) !=  5*Mega/PAGE_SIZE) panic("Wrong allocation:");
	cprintf("Test %d Passed \n", ++count);

	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames();
	tempAddress = kmalloc(4*Mega); 			   // Use Hole 4 -> Hole 4 = 1 M
	if((uint32)tempAddress != ACTUAL_START + 0x04100000)
		panic("Worst Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) !=  4*Mega/PAGE_SIZE) panic("Wrong allocation:");
	cprintf("Test %d Passed \n", ++count);

	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames();
	tempAddress = kmalloc(2 * Mega); 			// Use Hole 2 -> Hole 2 = 2 M
	if((uint32)tempAddress != ACTUAL_START + 0x00400000)
		panic("Worst Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) !=  2*Mega/PAGE_SIZE) panic("Wrong allocation:");
	cprintf("Test %d Passed \n", ++count);

	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames();
	tempAddress = kmalloc(1*Mega + 512*kilo);    // Use Hole 1 -> Hole 1 = 0.5 M
	if((uint32)tempAddress != ACTUAL_START)
		panic("Worst Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) !=  (1*Mega + 512*kilo)/PAGE_SIZE) panic("Wrong allocation:");
	cprintf("Test %d Passed \n", ++count);

	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames();
	tempAddress = kmalloc(512*kilo); 			   // Use Hole 2 -> Hole 2 = 1.5 M
	if((uint32)tempAddress != ACTUAL_START + 0x00600000)
		panic("Worst Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) !=  (512*kilo)/PAGE_SIZE) panic("Wrong allocation:");
	cprintf("Test %d Passed \n", ++count);

	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames();
	tempAddress = kmalloc(kilo); 			   // Use Hole 5 -> Hole 5 = 2 M - K
	if((uint32)tempAddress != ACTUAL_START + 0x09200000)
		panic("Worst Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) !=  (4*kilo)/PAGE_SIZE) panic("Wrong allocation:");
	cprintf("Test %d Passed \n", ++count);

	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames();
	tempAddress = kmalloc(2*Mega - 4*kilo); 		// Use Hole 5 -> Hole 5 = 0
	if((uint32)tempAddress != ACTUAL_START + 0x09201000)
		panic("Worst Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) !=  (2*Mega - 4*kilo)/PAGE_SIZE) panic("Wrong allocation:");
	cprintf("Test %d Passed \n", ++count);

	// Check that worst fit returns null in case all holes are not free
	freeFrames = sys_calculate_free_frames() ;
	freeDiskFrames = pf_calculate_free_frames();
	tempAddress = kmalloc(4*Mega); 		//No Suitable hole
	if((uint32)tempAddress != 0x0)
		panic("Worst Fit not working correctly");
	if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
	if ((freeFrames - sys_calculate_free_frames()) !=  0) panic("Wrong allocation:");
	cprintf("Test %d Passed \n", ++count);

	cprintf("Congratulations!! test Worst Fit completed successfully.\n");


	return 1;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}

int test_kfree()
{
<<<<<<< HEAD
	panic("not implemented");
=======
	panic("not handled yet after applying dynamic allocator with page allocator");

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	char *byteArr, *byteArr2 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = sys_calculate_free_frames() ;

	//malloc some spaces
	int i, freeFrames, freeDiskFrames ;
	char* ptr;
	int lastIndices[20] = {0};
	int sums[20] = {0};
	void* ptr_allocations[20] = {0};
	{
		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[0] !=  (ACTUAL_START)) panic("Wrong start address for the allocated space... check return address of kmalloc");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[0] = (2*Mega-kilo)/sizeof(char) - 1;

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 512) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[1] = (2*Mega-kilo)/sizeof(char) - 1;

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[2] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[2] != (ACTUAL_START + 4*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[2] = (2*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[2];
		for (i = 0; i < lastIndices[2]; ++i)
		{
			ptr[i] = 2 ;
		}

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[3] = kmalloc(2*kilo);
		if ((uint32) ptr_allocations[3] != (ACTUAL_START + 4*Mega + 4*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[3] = (2*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[3];
		for (i = 0; i < lastIndices[3]; ++i)
		{
			ptr[i] = 3 ;
		}

		//7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[4] = kmalloc(7*kilo);
		if ((uint32) ptr_allocations[4] != (ACTUAL_START + 4*Mega + 8*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 2) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[4] = (7*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[4];
		for (i = 0; i < lastIndices[4]; ++i)
		{
			ptr[i] = 4 ;
		}

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega + 16*kilo) ) panic("Wrong start address for the allocated space... check return address of kmalloc");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 768) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[5] = (3*Mega-kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[5];
		for (i = 0; i < lastIndices[5]; ++i)
		{
			ptr[i] = 5 ;
		}

		//6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(6*Mega-kilo);
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 7*Mega + 16*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 1536) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[6] = (6*Mega-kilo)/sizeof(char) - 1;

		//14 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(14*kilo);
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 13*Mega + 16*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 4) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[7] = (14*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[7];
		for (i = 0; i < lastIndices[7]; ++i)
		{
			ptr[i] = 7 ;
		}
	}

	//kfree some of the allocated spaces [15%]
	{
		//kfree 1st 2 MB
		int freeFrames = sys_calculate_free_frames() ;
		int freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 512 ) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 1st 2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[2]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 1 ) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 2nd 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 512) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[6]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 6*Mega/4096) panic("Wrong kfree: pages in memory are not freed correctly");
	}

	cprintf("\nkfree: current evaluation = 15%");

	//Check memory access after kfree [15%]
	{
		//2 KB
		ptr = (char*)ptr_allocations[3];
		for (i = 0; i < lastIndices[3]; ++i)
		{
			sums[3] += ptr[i] ;
		}
		if (sums[3] != 3*lastIndices[3])	panic("kfree: invalid read after freeing some allocations");

		//7 KB
		ptr = (char*)ptr_allocations[4];
		for (i = 0; i < lastIndices[4]; ++i)
		{
			sums[4] += ptr[i] ;
		}
		if (sums[4] != 4*lastIndices[4])	panic("kfree: invalid read after freeing some allocations");

		//3 MB
		ptr = (char*)ptr_allocations[5];
		for (i = 0; i < lastIndices[5]; ++i)
		{
			sums[5] += ptr[i] ;
		}
		if (sums[5] != 5*lastIndices[5])	panic("kfree: invalid read after freeing some allocations");

		//14 KB
		ptr = (char*)ptr_allocations[7];
		for (i = 0; i < lastIndices[7]; ++i)
		{
			sums[7] += ptr[i] ;
		}
		if (sums[7] != 7*lastIndices[7])	panic("kfree: invalid read after freeing some allocations");
	}
	cprintf("\b\b\b30%");

	//Allocate after kfree [15%]
	{
		//20 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[8] = kmalloc(20*kilo);
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 13*Mega + 32*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 5) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[8] = (20*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[8];
		for (i = 0; i < lastIndices[8]; ++i)
		{
			ptr[i] = 8 ;
		}

		//1 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[9] = kmalloc(1*Mega);
		if ((uint32) ptr_allocations[9] != (ACTUAL_START + 13*Mega + 52*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc");
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong allocation: pages are not loaded successfully into memory");
		lastIndices[9] = (1*Mega)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[9];
		for (i = 0; i < lastIndices[9]; ++i)
		{
			ptr[i] = 9 ;
		}

		if (isKHeapPlacementStrategyNEXTFIT())
		{
			//Allocate Remaining MBs
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			uint32 reqAllocatedSpace = KERNEL_HEAP_MAX - (ACTUAL_START + 13*Mega + 52*kilo + 1*Mega);
			ptr_allocations[10] = kmalloc(reqAllocatedSpace);
			if ((uint32) ptr_allocations[10] != (ACTUAL_START + 13*Mega + 52*kilo + 1*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc");
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
			if ((freeFrames - sys_calculate_free_frames()) != reqAllocatedSpace/PAGE_SIZE) panic("Wrong allocation: pages are not loaded successfully into memory");
			lastIndices[10] = (reqAllocatedSpace)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[10];
			//			for (i = 0; i < lastIndices[10]; ++i)
			//			{
			//				ptr[i] = 10;
			//			}

			//Allocate in merged freed space FROM the beginning
			//3 MB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[11] = kmalloc(3*Mega);
			if ((uint32) ptr_allocations[11] != (ACTUAL_START)) panic("Wrong start address for the allocated space... check return address of kmalloc");
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
			if ((freeFrames - sys_calculate_free_frames()) != 768) panic("Wrong allocation: pages are not loaded successfully into memory");
			lastIndices[11] = (3*Mega)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[11];
			for (i = 0; i < lastIndices[11]; ++i)
			{
				ptr[i] = 8 ;
			}

			//2 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[12] = kmalloc(2*kilo);
			if ((uint32) ptr_allocations[12] != (ACTUAL_START + 3*Mega)) panic("Wrong start address for the allocated space... check return address of kmalloc");
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
			if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: pages are not loaded successfully into memory");
			lastIndices[12] = (2*kilo)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[12];
			for (i = 0; i < lastIndices[12]; ++i)
			{
				ptr[i] = 9 ;
			}

			//1 MB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[13] = kmalloc(1*Mega);
			if ((uint32) ptr_allocations[13] != (ACTUAL_START + 3*Mega + 4*kilo)) panic("Wrong start address for the allocated space... check return address of kmalloc");
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
			if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong allocation: pages are not loaded successfully into memory");
			lastIndices[13] = (1*Mega)/sizeof(char) - 1;
			ptr = (char*)ptr_allocations[13];
			for (i = 0; i < lastIndices[13]; ++i)
			{
				ptr[i] = 10 ;
			}
		}
	}
	cprintf("\b\b\b45%");

	//kfree remaining allocated spaces [15%]
	{
		//kfree 7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[4]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 2) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[5]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 3*Mega/4096) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 2nd 2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[3]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 1) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 14 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[7]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 4) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 20 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[8]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 5) panic("Wrong kfree: pages in memory are not freed correctly");

		//kfree 1 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[9]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 256) panic("Wrong kfree: pages in memory are not freed correctly");

		if (isKHeapPlacementStrategyNEXTFIT())
		{
			//cprintf("FREE in NEXT FIT\n");
			//kfree Remaining MBs
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			kfree(ptr_allocations[10]);
			uint32 reqAllocatedSpace = KERNEL_HEAP_MAX - (ACTUAL_START + 13*Mega + 52*kilo + 1*Mega);
			if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
			if ((sys_calculate_free_frames() - freeFrames) != reqAllocatedSpace/PAGE_SIZE) panic("Wrong kfree: pages in memory are not freed correctly");

			//kfree 3 MB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			kfree(ptr_allocations[11]);
			if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
			if ((sys_calculate_free_frames() - freeFrames) != 3*Mega/4096) panic("Wrong kfree: pages in memory are not freed correctly");

			//kfree 2 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			kfree(ptr_allocations[12]);
			if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
			if ((sys_calculate_free_frames() - freeFrames) != 1) panic("Wrong kfree: pages in memory are not freed correctly");

			//kfree 1 MB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			kfree(ptr_allocations[13]);
			if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
			if ((sys_calculate_free_frames() - freeFrames) != 1*Mega/4096) panic("Wrong kfree: pages in memory are not freed correctly");

		}
		if(start_freeFrames != (sys_calculate_free_frames())) {panic("Wrong kfree: not all pages removed correctly at end");}
	}
	cprintf("\b\b\b60%");

	//Check memory access after kfree [15%]
	{
		//Bypass the PAGE FAULT on <MOVB immediate, reg> instruction by setting its length
		//and continue executing the remaining code
		sys_bypassPageFault(3);

		for (i = 0; i < 10; ++i)
		{
			ptr = (char *) ptr_allocations[i];
			ptr[0] = 10;
			//cprintf("\n\ncr2 = %x, faulted addr = %x", sys_rcr2(), (uint32)&(ptr[0]));
			if (sys_rcr2() != (uint32)&(ptr[0])) panic("kfree: successful access to freed space!! it should not be succeeded");
			ptr[lastIndices[i]] = 10;
			if (sys_rcr2() != (uint32)&(ptr[lastIndices[i]])) panic("kfree: successful access to freed space!! it should not be succeeded");
		}

		//set it to 0 again to cancel the bypassing option
		sys_bypassPageFault(0);
	}
	cprintf("\b\b\b75%");

	//kfree non-exist item [10%]
	{
		//kfree 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing");

		//kfree 2 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[2]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing");

		//kfree 20 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[8]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing");

		//kfree 1 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[9]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing");

	}
	cprintf("\b\b\b85%");

	//check tables	[15%]
	{
		long long va;
		for (va = KERNEL_HEAP_START; va < (long long)KERNEL_HEAP_MAX; va+=PTSIZE)
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, (uint32)va, &ptr_table);
			if (ptr_table == NULL)
			{
				panic("Wrong kfree: one of the kernel tables is wrongly removed! Tables should not be removed here in kfree");
			}
		}
	}

	cprintf("\b\b\b100%\n");



	cprintf("\nCongratulations!! test kfree completed successfully.\n");

	return 1;

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}


int initFreeFrames;
int initFreeDiskFrames ;
uint8 firstCall = 1 ;
int test_three_creation_functions()
{
	if (firstCall)
	{
		firstCall = 0;
		initFreeFrames = sys_calculate_free_frames() ;
		initFreeDiskFrames = pf_calculate_free_frames() ;
		//Run simple user program
		{
			char command[100] = "run fos_add 4096";
			execute_command(command) ;
		}
	}
	//Ensure that the user directory, page WS and page tables are allocated in KERNEL HEAP
	{
		struct Env * e = NULL;
		struct Env * ptr_env = NULL;
		LIST_FOREACH(ptr_env, &ProcessQueues.env_exit_queue)
		{
			if (strcmp(ptr_env->prog_name, "fos_add") == 0)
			{
				e = ptr_env ;
				break;
			}
		}
		if (e->pageFaultsCounter != 0)
			panic("Page fault is occur while not expected to. Review the three creation functions");

#if USE_KHEAP
		int pagesInWS = LIST_SIZE(&(e->page_WS_list));
#else
		int pagesInWS = env_page_ws_get_size(e);
#endif
		int curFreeFrames = sys_calculate_free_frames() ;
		int curFreeDiskFrames = pf_calculate_free_frames() ;
		//cprintf("\ndiff in page file = %d, pages in WS = %d\n", initFreeDiskFrames - curFreeDiskFrames, pagesInWS);
		if ((initFreeDiskFrames - curFreeDiskFrames) != pagesInWS) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		//cprintf("\ndiff in mem frames = %d, pages in WS = %d\n", initFreeFrames - curFreeFrames, pagesInWS);
		if ((initFreeFrames - curFreeFrames) != 12/*WS*/ + 2*1/*DIR*/ + 2*3/*Tables*/ + 1 /*user WS table*/ + pagesInWS) panic("Wrong allocation: pages are not loaded successfully into memory");

		//allocate 4 KB
		char *ptr = kmalloc(4*kilo);
		if ((uint32) ptr !=  (ACTUAL_START + (12+2*1+2*3+1)*PAGE_SIZE)) panic("Wrong start address for the allocated space... make sure you create the dir, table and page WS in KERNEL HEAP");
	}

	cprintf("\nCongratulations!! test the 3 creation functions is completed successfully.\n");

	return 1;
}



extern void kfreeall() ;

int test_kfreeall()
{
<<<<<<< HEAD
	panic("not implemented");
}


extern void kexpand(uint32 newSize) ;

int test_kexpand()
{
	panic("not implemented");
}

extern void kshrink(uint32 newSize) ;

int test_kshrink()
{
	panic("not implemented");

}


int test_kfreelast()
{
	panic("not implemented");

=======
	panic("test not available yet");

	return 1;

}
int test_kexpand(){
	panic("test not available yet");
	return 1;
}

int test_kshrink(){
	panic("test not available yet");
	return 1;
}
int test_kfreelast(){
	panic("test not available yet");
	return 1;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}

int test_krealloc() {
	cprintf("==============================================\n");
<<<<<<< HEAD
	    cprintf(
	            "MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	    cprintf("==============================================\n");
	    panic("test not available yet");
	    return 0;
=======
	cprintf(
			"MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");
	panic("test not available yet");
	return 0;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}


int test_krealloc_BF() {
<<<<<<< HEAD
	panic("not implemented");

=======
	cprintf("==============================================\n");
	cprintf(
			"MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");
	panic("test not available yet");
	return 0;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}

int test_krealloc_FF1()
{
<<<<<<< HEAD
	panic("not implemented");
=======
	cprintf("===================================================\n");
	cprintf("*****NOTE: THIS IS A COMPLETE TEST FOR KREALLOC [BLOCK ALLOCATOR]******\n") ;
	cprintf("===================================================\n");

	panic("test not available yet");
	return 0;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

}
int test_krealloc_FF2()
{
<<<<<<< HEAD
	panic("not implemented");

=======
	cprintf("===================================================\n");
	cprintf("*****NOTE: THIS IS A COMPLETE TEST FOR KREALLOC [PAGE ALLOCATOR]******\n") ;
	cprintf("===================================================\n");

	panic("test not available yet");
	return 0;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}

int test_krealloc_FF3()
{
<<<<<<< HEAD
	panic("not implemented");

=======
	cprintf("===================================================\n");
	cprintf("*****NOTE: THIS IS A COMPLETE TEST FOR KREALLOC [SWITCH FROM PAGE ALLOCATOR TO DYNAMIC ALLOCATOR AND VICE VERSA]******\n") ;
	cprintf("===================================================\n");

	panic("test not available yet");
	return 0;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}


