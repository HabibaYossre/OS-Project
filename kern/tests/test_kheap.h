/*
 * test_kheap.h
 *
 *  Created on: Oct 14, 2022
 *      Author: HP
 */

#ifndef KERN_TESTS_TEST_KHEAP_H_
#define KERN_TESTS_TEST_KHEAP_H_
#ifndef FOS_KERNEL
# error "This is a FOS kernel header; user programs should not #include it"
#endif
<<<<<<< HEAD
#include <inc/types.h>
=======

>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
//2016: Kernel Heap Tests
 int test_kmalloc();
 int test_kmalloc_nextfit();
 int test_kmalloc_bestfit1();
 int test_kmalloc_bestfit2();
 int test_kmalloc_firstfit1();
 int test_kmalloc_firstfit2();
<<<<<<< HEAD
 int test_fastfirstfit();
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
 int test_kmalloc_worstfit();
 int test_kfree();
 int test_kfree_bestfirstfit();
 int test_kheap_phys_addr();
 int test_kheap_virt_addr();
 int test_three_creation_functions();
 int test_kfreeall();
 int test_kexpand();
 int test_kshrink();
 int test_kfreelast();
 int test_krealloc();
 int test_krealloc_BF();
 int test_ksbrk();
 int test_krealloc_FF1();
 int test_krealloc_FF2();
 int test_krealloc_FF3();
<<<<<<< HEAD
 int check_block(void* va, void* expectedVA, uint32 expectedSize, uint8 expectedFlag);
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

 //2022
 int test_initialize_dyn_block_system(int freeFrames_before, int freeDiskFrames_before, int freeFrames_after, int freeDiskFrames_after);

#endif /* KERN_TESTS_TEST_KHEAP_H_ */
