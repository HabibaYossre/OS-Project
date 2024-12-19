// User-level Semaphore

#include "inc/lib.h"

<<<<<<< HEAD

struct semaphore create_semaphore(char *semaphoreName, uint32 value)
{
//	//TODO: [PROJECT'24.MS3 - #02] [2] USER-LEVEL SEMAPHORE - create_semaphore
//	//COMMENT THE FOLLOWING LINE BEFORE START CODING
//	panic("create_semaphore is not implemented yet");
//	//Your Code is Here...
	//void* smalloc(char* sharedVarName, uint32 size, uint8 isWritable)

	 struct semaphore sem;
	 sem.semdata = NULL;
	 sem.semdata=(struct __semdata* )smalloc(semaphoreName,sizeof(struct semaphore),1);

	 // or semdata
	    if (sem.semdata == NULL)
	    {
	        panic("Failed to allocate memory for semaphore");
	    }

	    strcpy(sem.semdata->name, semaphoreName);
	    sem.semdata->count = value;
	    sys_init_queue(&(sem.semdata->queue));
	    sem.semdata->lock = 0;

	    return sem;
}
struct semaphore get_semaphore(int32 ownerEnvID, char* semaphoreName)
{
//	//TODO: [PROJECT'24.MS3 - #03] [2] USER-LEVEL SEMAPHORE - get_semaphore
//	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("get_semaphore is not implemented yet");
//	//Your Code is Here...

	struct semaphore sem;
	sem.semdata= (struct __semdata* ) sget(ownerEnvID,semaphoreName);

	if (sem.semdata == NULL)
	{
		 panic("Failed to get semaphore");
	}
	return sem;
=======
struct semaphore create_semaphore(char *semaphoreName, uint32 value)
{
	//[PROJECT'24.MS3]
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	panic("create_semaphore is not implemented yet");
	//Your Code is Here...
}
struct semaphore get_semaphore(int32 ownerEnvID, char* semaphoreName)
{
	//[PROJECT'24.MS3]
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	panic("get_semaphore is not implemented yet");
	//Your Code is Here...
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}

void wait_semaphore(struct semaphore sem)
{
<<<<<<< HEAD
	//TODO: [PROJECT'24.MS3 - #04] [2] USER-LEVEL SEMAPHORE - wait_semaphore
	//panic("wait_semaphore is not implemented yet");

	 	 /*The semWait operation
	     * 1. Decrements the semaphore value.
	     * 2. If the value becomes negative, the process executing the semWait is blocked.
	     * 3. Otherwise, the process continues execution.*/

  		//Acquire the lock
		while(xchg(&(sem.semdata->lock),1) != 0);

		sem.semdata->count--;
		if (sem.semdata->count < 0)
		{
			/* place this process in s.queue */
			/* block this process */
			sys_wait_semaphore(&(sem.semdata->queue),&(sem.semdata->lock));
		}

		/*Release Lock*/
		sem.semdata->lock=0;

}
void signal_semaphore(struct semaphore sem)
{
	     //TODO: [PROJECT'24.MS3 - #05] [2] USER-LEVEL SEMAPHORE - signal_semaphore

	    // 1. The semSignal operation increments the semaphore value.
	    // 2. If the resulting is less than or equal to zero, then a process blocked by a semWait operation,
	   //if any, is unblocked.

	    /*acquire*/
		while(xchg(&(sem.semdata->lock),1) != 0);

			sem.semdata->count++;
			if(sem.semdata->count<=0) // there are some threads in the queue want to quit to be ready .
			{
				/*Remove process p from sem.queue*/
				/*place process p on ready list*/

				sys_signal_semaphore(&(sem.semdata->queue),&(sem.semdata->lock));
			}

			/*release*/
			sem.semdata->lock = 0;
=======
	//[PROJECT'24.MS3]
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	panic("wait_semaphore is not implemented yet");
	//Your Code is Here...
}

void signal_semaphore(struct semaphore sem)
{
	//[PROJECT'24.MS3]
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	panic("signal_semaphore is not implemented yet");
	//Your Code is Here...
>>>>>>> c561abf376cfb4d393cdf60026fa31c8d4beef8c
}

int semaphore_count(struct semaphore sem)
{
	return sem.semdata->count;
}
