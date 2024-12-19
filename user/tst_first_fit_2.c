/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 10000 */
/* *********************************************************** */

#include <inc/lib.h>
<<<<<<< HEAD
#include <user/tst_utilities.h>
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

#define Mega  (1024*1024)
#define kilo (1024)
#define numOfAllocs 7
#define allocCntPerSize 200
#define sizeOfMetaData 8

/*NOTE: these sizes include the size of MetaData within it &
 * ALL sizes are in increasing order to ensure that they allocated contiguously
 * & no later block can be allocated in a previous unused free block at the end of page
 */
uint32 allocSizes[numOfAllocs] = {3*sizeof(int) + sizeOfMetaData, 2*sizeOfMetaData, 20*sizeof(char) + sizeOfMetaData, kilo/2, 1*kilo, 3*kilo/2, 2*kilo} ;
short* startVAs[numOfAllocs*allocCntPerSize+1] ;
short* midVAs[numOfAllocs*allocCntPerSize+1] ;
short* endVAs[numOfAllocs*allocCntPerSize+1] ;

void _main(void)
{
	/*********************** NOTE ****************************
	 * WE COMPARE THE DIFF IN FREE FRAMES BY "AT LEAST" RULE
	 * INSTEAD OF "EQUAL" RULE SINCE IT'S POSSIBLE THAT SOME
	 * PAGES ARE ALLOCATED IN KERNEL DYNAMIC ALLOCATOR sbrk()
	 * (e.g. DURING THE DYNAMIC CREATION OF WS ELEMENT in FH).
	 *********************************************************/
	sys_set_uheap_strategy(UHP_PLACE_FIRSTFIT);

	//cprintf("1\n");
	//Initial test to ensure it works on "PLACEMENT" not "REPLACEMENT"
#if USE_KHEAP
	{
		if (LIST_SIZE(&(myEnv->page_WS_list)) >= myEnv->page_WS_max_size)
			panic("Please increase the WS size");
	}
#else
	panic("make sure to enable the kernel heap: USE_KHEAP=1");
#endif

	/*=================================================*/

	int eval = 0;
	bool is_correct = 1;
	int targetAllocatedSpace = 3*Mega;

	void * va ;
	int idx = 0;
	bool chk;
	int usedDiskPages = sys_pf_calculate_allocated_pages() ;
	int freeFrames = sys_calculate_free_frames() ;
<<<<<<< HEAD
	uint32 actualSize, block_size, blockIndex;
	int8 block_status;
	void* expectedVA;
	uint32 expectedSize, curTotalSize,roundedTotalSize ;

	void* curVA = (void*) USER_HEAP_START + sizeof(int) /*BEG Block*/ ;
=======
	uint32 actualSize;

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	//====================================================================//
	/*INITIAL ALLOC Scenario 1: Try to allocate set of blocks with different sizes*/
	cprintf("PREREQUISITE#1: Try to allocate set of blocks with different sizes [all should fit]\n\n") ;
	{
		is_correct = 1;
<<<<<<< HEAD
		curTotalSize = sizeof(int);
=======
		void* curVA = (void*) USER_HEAP_START ;
		uint32 actualSize;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		for (int i = 0; i < numOfAllocs; ++i)
		{
			for (int j = 0; j < allocCntPerSize; ++j)
			{
				actualSize = allocSizes[i] - sizeOfMetaData;
<<<<<<< HEAD
				va = startVAs[idx] = malloc(actualSize);
				midVAs[idx] = va + actualSize/2 ;
				endVAs[idx] = va + actualSize - sizeof(short);
				//Check returned va
				expectedVA = (curVA + sizeOfMetaData/2);
				expectedSize = allocSizes[i];
				curTotalSize += allocSizes[i] ;
				//============================================================
				//Check if the remaining area doesn't fit the DynAllocBlock,
				//so update the curVA & curTotalSize to skip this area
				roundedTotalSize = ROUNDUP(curTotalSize, PAGE_SIZE);
				int diff = (roundedTotalSize - curTotalSize) ;
				if (diff > 0 && diff < (DYN_ALLOC_MIN_BLOCK_SIZE + sizeOfMetaData))
				{
//					cprintf("%~\n FRAGMENTATION: curVA = %x diff = %d\n", curVA, diff);
//					cprintf("%~\n Allocated block @ %x with size = %d\n", va, get_block_size(va));

					curVA = ROUNDUP(curVA, PAGE_SIZE)- sizeof(int) /*next alloc will start at END Block (after sbrk)*/;
					curTotalSize = roundedTotalSize - sizeof(int) /*exclude END Block*/;
					expectedSize += diff - sizeof(int) /*exclude END Block*/;
				}
				else
				{
					curVA += allocSizes[i] ;
				}
				//============================================================
				if (is_correct)
				{
					if (check_block(va, expectedVA, expectedSize, 1) == 0)
					{
						if (is_correct)
						{
							is_correct = 0;
							panic("alloc_block_xx #PRQ.%d: WRONG ALLOC\n", idx);
						}
					}
				}
				idx++;

=======
				va = startVAs[idx++] = malloc(actualSize);
				//Check returned va
				if(va == NULL || (va < curVA))
				{
					if (is_correct)
					{
						is_correct = 0;
						panic("malloc() #1.%d: WRONG ALLOC - alloc_block_FF return wrong address. Expected %x, Actual %x\n", idx, curVA + sizeOfMetaData ,va);
					}
				}
				curVA += allocSizes[i] ;

				//============================================================
				//Check if the remaining area doesn't fit the DynAllocBlock,
				//so update the curVA to skip this area
				void* rounded_curVA = ROUNDUP(curVA, PAGE_SIZE);
				int diff = (rounded_curVA - curVA) ;
				if (diff > 0 && diff < sizeOfMetaData)
				{
					curVA = rounded_curVA;
				}
				//============================================================
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			}
			//if (is_correct == 0)
			//break;
		}
	}
<<<<<<< HEAD
	/* Fill the remaining space at the end of the DA*/
	roundedTotalSize = ROUNDUP(curTotalSize, PAGE_SIZE);
	uint32 remainSize = (roundedTotalSize - curTotalSize) - sizeof(int) /*END block*/;
	if (remainSize >= (DYN_ALLOC_MIN_BLOCK_SIZE + sizeOfMetaData))
	{
		cprintf("Filling the remaining size of %d\n\n", remainSize);
		va = startVAs[idx] = alloc_block(remainSize - sizeOfMetaData, DA_FF);
		//Check returned va
		expectedVA = curVA + sizeOfMetaData/2;
		if (check_block(va, expectedVA, remainSize, 1) == 0)
		{
			is_correct = 0;
			panic("alloc_block_xx #PRQ.oo: WRONG ALLOC\n", idx);
		}
	}
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

	//====================================================================//
	/*Free set of blocks with different sizes (first block of each size)*/
	cprintf("1: Free set of blocks with different sizes (first block of each size)\n\n") ;
	{
<<<<<<< HEAD
		is_correct = 1;
		for (int i = 0; i < numOfAllocs; ++i)
		{
			free(startVAs[i*allocCntPerSize]);
			if (check_block(startVAs[i*allocCntPerSize], startVAs[i*allocCntPerSize], allocSizes[i], 0) == 0)
			{
				is_correct = 0;
				cprintf("test_ff_2 #1.1: WRONG FREE!\n");
			}
=======
		for (int i = 0; i < numOfAllocs; ++i)
		{
			free(startVAs[i*allocCntPerSize]);
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		}
	}

#define numOfFFTests 3
	short* tstStartVAs[numOfFFTests+1] ;
	short* tstMidVAs[numOfFFTests+1] ;
	short* tstEndVAs[numOfFFTests+1] ;

	//====================================================================//
	/*FF ALLOC Scenario 2: Try to allocate blocks with sizes smaller than existing free blocks*/
	cprintf("2: Try to allocate set of blocks with different sizes smaller than existing free blocks\n\n") ;

	uint32 testSizes[numOfFFTests] =
	{
			kilo/4,								//expected to be allocated in 4th free block
			8*sizeof(char) + sizeOfMetaData, 	//expected to be allocated in 1st free block
			kilo/8,								//expected to be allocated in remaining of 4th free block
	} ;
<<<<<<< HEAD
	uint32 expectedSizes[numOfFFTests] =
	{
			kilo/4,					//expected to be allocated in 4th free block
			allocSizes[0], 			//INTERNAL FRAGMENTATION CASE in 1st Block
			kilo/8,					//expected to be allocated in remaining of 4th free block
	} ;
=======

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	uint32 startOf1stFreeBlock = (uint32)startVAs[0*allocCntPerSize];
	uint32 startOf4thFreeBlock = (uint32)startVAs[3*allocCntPerSize];

	{
		is_correct = 1;

		uint32 expectedVAs[numOfFFTests] =
		{
				startOf4thFreeBlock,
				startOf1stFreeBlock,
				startOf4thFreeBlock + testSizes[0]
		};
		for (int i = 0; i < numOfFFTests; ++i)
		{
			actualSize = testSizes[i] - sizeOfMetaData;
			va = tstStartVAs[i] = malloc(actualSize);
			tstMidVAs[i] = va + actualSize/2 ;
			tstEndVAs[i] = va + actualSize - sizeof(short);
			//Check returned va
<<<<<<< HEAD
			if (check_block(tstStartVAs[i], (void*) expectedVAs[i], expectedSizes[i], 1) == 0)
			{
				is_correct = 0;
				cprintf("test_ff_2 #2.%d: WRONG ALLOCATE AFTER FREE!\n", i);
=======
			if(tstStartVAs[i] == NULL || (tstStartVAs[i] != (short*)expectedVAs[i]))
			{
				is_correct = 0;
				cprintf("malloc() #2.%d: WRONG FF ALLOC - alloc_block_FF return wrong address. Expected %x, Actual %x\n", i, expectedVAs[i] ,tstStartVAs[i]);
				//break;
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			}
			*(tstStartVAs[i]) = 353 + i;
			*(tstMidVAs[i]) = 353 + i;
			*(tstEndVAs[i]) = 353 + i;
		}
		//Check stored sizes
		if(get_block_size(tstStartVAs[1]) != allocSizes[0])
		{
			is_correct = 0;
<<<<<<< HEAD
			cprintf("test_ff_2 #2.3: WRONG FF ALLOC - make sure if the remaining free space doesn�t fit a dynamic allocator block, then this area should be added to the allocated area and counted as internal fragmentation\n");
=======
			cprintf("malloc() #3: WRONG FF ALLOC - make sure if the remaining free space doesn�t fit a dynamic allocator block, then this area should be added to the allocated area and counted as internal fragmentation\n");
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
			//break;
		}
		if (is_correct)
		{
			eval += 30;
		}
	}

	//====================================================================//
	/*FF ALLOC Scenario 3: Try to allocate a block with a size equal to the size of the first existing free block*/
	cprintf("3: Try to allocate a block with equal to the first existing free block\n\n") ;
	{
		is_correct = 1;

		actualSize = kilo/8 - sizeOfMetaData; 	//expected to be allocated in remaining of 4th free block
<<<<<<< HEAD
		expectedSize = ROUNDUP(actualSize + sizeOfMetaData, 2);
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		va = tstStartVAs[numOfFFTests] = malloc(actualSize);
		tstMidVAs[numOfFFTests] = va + actualSize/2 ;
		tstEndVAs[numOfFFTests] = va + actualSize - sizeof(short);
		//Check returned va
<<<<<<< HEAD
		expectedVA = (void*)(startOf4thFreeBlock + testSizes[0] + testSizes[2]) ;
		if (check_block(tstStartVAs[numOfFFTests], expectedVA, expectedSize, 1) == 0)
		{
			is_correct = 0;
			cprintf("test_ff_2 #3: WRONG ALLOCATE AFTER FREE!\n");
=======
		void* expected = (void*)(startOf4thFreeBlock + testSizes[0] + testSizes[2]) ;
		if(va == NULL || (va != expected))
		{
			is_correct = 0;
			cprintf("malloc() #4: WRONG FF ALLOC - alloc_block_FF return wrong address.expected %x, actual %x\n", expected, va);
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		}
		*(tstStartVAs[numOfFFTests]) = 353 + numOfFFTests;
		*(tstMidVAs[numOfFFTests]) = 353 + numOfFFTests;
		*(tstEndVAs[numOfFFTests]) = 353 + numOfFFTests;

		if (is_correct)
		{
			eval += 30;
		}
	}
	//====================================================================//
	/*FF ALLOC Scenario 4: Check stored data inside each allocated block*/
	cprintf("4: Check stored data inside each allocated block\n\n") ;
	{
		is_correct = 1;

		for (int i = 0; i <= numOfFFTests; ++i)
		{
			//cprintf("startVA = %x, mid = %x, last = %x\n", tstStartVAs[i], tstMidVAs[i], tstEndVAs[i]);
			if (*(tstStartVAs[i]) != (353+i) || *(tstMidVAs[i]) != (353+i) || *(tstEndVAs[i]) != (353+i) )
			{
				is_correct = 0;
<<<<<<< HEAD
				cprintf("malloc #4.%d: WRONG! content of the block is not correct. Expected=%d, val1=%d, val2=%d, val3=%d\n",i, (353+i), *(tstStartVAs[i]), *(tstMidVAs[i]), *(tstEndVAs[i]));
=======
				cprintf("malloc #5.%d: WRONG! content of the block is not correct. Expected=%d, val1=%d, val2=%d, val3=%d\n",i, (353+i), *(tstStartVAs[i]), *(tstMidVAs[i]), *(tstEndVAs[i]));
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
				break;
			}
		}

		if (is_correct)
		{
			eval += 20;
		}
	}

	//====================================================================//
	/*FF ALLOC Scenario 5: Test a Non-Granted Request */
	cprintf("5: Test a Non-Granted Request\n\n") ;
	{
		is_correct = 1;
		actualSize = 2*kilo - sizeOfMetaData;

		//Fill the 7th free block
		va = malloc(actualSize);

		//Fill the remaining area
		uint32 numOfRem2KBAllocs = ((USER_HEAP_START + DYN_ALLOC_MAX_SIZE - (uint32)sbrk(0)) / PAGE_SIZE) * 2;
		for (int i = 0; i < numOfRem2KBAllocs; ++i)
		{
			va = malloc(actualSize);
			if(va == NULL)
			{
				is_correct = 0;
<<<<<<< HEAD
				cprintf("malloc() #5.%d: WRONG FF ALLOC - alloc_block_FF return NULL address while it's expected to return correct one.\n");
=======
				cprintf("malloc() #6.%d: WRONG FF ALLOC - alloc_block_FF return NULL address while it's expected to return correct one.\n");
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
				break;
			}
		}

		//Test two more allocs
		va = malloc(actualSize);
		va = malloc(actualSize);

		if(va != NULL)
		{
			is_correct = 0;
<<<<<<< HEAD
			cprintf("malloc() #6: WRONG FF ALLOC - alloc_block_FF return an address while it's expected to return NULL since it reaches the hard limit.\n");
=======
			cprintf("malloc() #7: WRONG FF ALLOC - alloc_block_FF return an address while it's expected to return NULL since it reaches the hard limit.\n");
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
		}
		if (is_correct)
		{
			eval += 20;
		}
	}
	cprintf("test FIRST FIT (2) [DYNAMIC ALLOCATOR] is finished. Evaluation = %d%\n", eval);

	return;
}
