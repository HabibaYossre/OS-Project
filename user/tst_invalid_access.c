/********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 15 - SECOND CHANCE LIST = 2*/
/************************************************************/

#include <inc/lib.h>

void _main(void)
{
	int eval = 0;

<<<<<<< HEAD
	cprintf("PART I: Test the Pointer Validation inside fault_handler(): [70%]\n");
	cprintf("=================================================================\n");
=======
	cprintf("PART I: Test the Pointer Validation inside fault_handler():\n");
	cprintf("===========================================================\n");
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	rsttst();
	int ID1 = sys_create_env("tia_slave1", (myEnv->page_WS_max_size), (myEnv->SecondListSize),(myEnv->percentage_of_WS_pages_to_be_removed));
	sys_run_env(ID1);

	int ID2 = sys_create_env("tia_slave2", (myEnv->page_WS_max_size), (myEnv->SecondListSize),(myEnv->percentage_of_WS_pages_to_be_removed));
	sys_run_env(ID2);

	int ID3 = sys_create_env("tia_slave3", (myEnv->page_WS_max_size), (myEnv->SecondListSize),(myEnv->percentage_of_WS_pages_to_be_removed));
	sys_run_env(ID3);
	env_sleep(10000);

	if (gettst() != 0)
		cprintf("\nPART I... Failed.\n");
	else
	{
		cprintf("\nPART I... completed successfully\n\n");
		eval += 70;
	}

<<<<<<< HEAD
	cprintf("PART II: PLACEMENT: Test the Invalid Access to a NON-EXIST page in Page File, Stack & Heap: [30%]\n");
	cprintf("=================================================================================================\n");
=======
	cprintf("PART II: PLACEMENT: Test the Invalid Access to a NON-EXIST page in Page File, Stack & Heap:\n");
	cprintf("===========================================================================================\n");
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

	rsttst();
	int ID4 = sys_create_env("tia_slave4", (myEnv->page_WS_max_size), (myEnv->SecondListSize),(myEnv->percentage_of_WS_pages_to_be_removed));
	sys_run_env(ID4);

	env_sleep(10000);

	if (gettst() != 0)
		cprintf("\nPART II... Failed.\n");
	else
	{
		cprintf("\nPART II... completed successfully\n\n");
		eval += 30;
	}
<<<<<<< HEAD
	cprintf("%~\ntest invalid access completed. Eval = %d\n\n", eval);
=======
	//cprintf("Congratulations... test invalid access completed successfully\n");
	cprintf("[AUTO_GR@DING_PARTIAL]%d\n", eval);
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

}

