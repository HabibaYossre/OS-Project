// Called from entry.S to get us going.
// entry.S already took care of defining envs, pages, vpd, and vpt.

#include <inc/lib.h>

extern void _main(int argc, char **argv);

volatile struct Env *myEnv = NULL;
<<<<<<< HEAD
volatile bool printStats = 1;

=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
volatile char *binaryname = "(PROGRAM NAME UNKNOWN)";
void
libmain(int argc, char **argv)
{
<<<<<<< HEAD
	//printStats = 1;
=======
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
	int envIndex = sys_getenvindex();

	myEnv = &(envs[envIndex]);

	//SET THE PROGRAM NAME
	if (myEnv->prog_name[0] != '\0')
		binaryname = myEnv->prog_name;

	// set env to point at our env structure in envs[].
	// env = envs;

	// save the name of the program so that panic() can use it
	if (argc > 0)
		binaryname = argv[0];

	// call user main routine
	_main(argc, argv);

<<<<<<< HEAD
	if (printStats)
	{
		sys_lock_cons();
		{
			cprintf("**************************************\n");
			cprintf("Num of PAGE faults = %d, modif = %d\n", myEnv->pageFaultsCounter, myEnv->nModifiedPages);
			cprintf("# PAGE IN (from disk) = %d, # PAGE OUT (on disk) = %d, # NEW PAGE ADDED (on disk) = %d\n", myEnv->nPageIn, myEnv->nPageOut,myEnv->nNewPageAdded);
			//cprintf("Num of freeing scarce memory = %d, freeing full working set = %d\n", myEnv->freeingScarceMemCounter, myEnv->freeingFullWSCounter);
			cprintf("Num of clocks = %d\n", myEnv->nClocks);
			cprintf("**************************************\n");
		}
		sys_unlock_cons();
	}
=======


	//	sys_lock_cons();
	sys_lock_cons();
	{
		cprintf("**************************************\n");
		cprintf("Num of PAGE faults = %d, modif = %d\n", myEnv->pageFaultsCounter, myEnv->nModifiedPages);
		cprintf("# PAGE IN (from disk) = %d, # PAGE OUT (on disk) = %d, # NEW PAGE ADDED (on disk) = %d\n", myEnv->nPageIn, myEnv->nPageOut,myEnv->nNewPageAdded);
		//cprintf("Num of freeing scarce memory = %d, freeing full working set = %d\n", myEnv->freeingScarceMemCounter, myEnv->freeingFullWSCounter);
		cprintf("Num of clocks = %d\n", myEnv->nClocks);
		cprintf("**************************************\n");
	}
	sys_unlock_cons();
//	sys_unlock_cons();
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c

	// exit gracefully
	exit();
}

