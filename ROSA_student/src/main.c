
#include "kernel/rosa_sem.h"



int main(void)
{	
	handleID sem1, sem2, sem3, semIPCP1, semIPCP2, semIPCP3;
	TimerTick ten, nowaiting, forever;
	bool val;
	ROSA_init();
	// CREATING SEMAPHORES + NESTING + DELETION
	int i;
	for(i=0; i<MAX_SEM; i++)
	{
		semaphoreHandlerTable[i] = NULL;
	}

	ten = 10;
	nowaiting = 0;
	forever =  TIMERTICK_MAXVAL;

	sem1 = ROSA_SemaphoreBinaryCreate();
	sem2 = ROSA_SemaphoreBinaryCreate();
	sem3 = ROSA_SemaphoreBinaryCreate();
	
	ledOn(LED4_GPIO);

	if (sem1 != -1)
	{
		ledOn(LED0_GPIO);
	}
	
	if (sem2 != -1)
	{
		ledOn(LED1_GPIO);
	}
	
	if (sem3 != -1)
	{
		ledOn(LED2_GPIO);
	}

/***********************************************************
	ledOff(LED0_GPIO);
	ledOff(LED1_GPIO);
	ledOff(LED2_GPIO);


	val = ROSA_SemaphoreBinaryTake(sem1, ten);
	if (val == true)
	{
		ledOn(LED0_GPIO);
	}
	val = ROSA_SemaphoreBinaryTake(sem2, ten);
	if (val == true)
	{
	ledOn(LED1_GPIO);
	}

	val = ROSA_SemaphoreBinaryRelease (sem2);
	if (val == true)
	{
		ledOff(LED1_GPIO);
	}

	val = ROSA_SemaphoreBinaryRelease (sem1);
	if (val == true)
	{
		ledOff(LED2_GPIO);
	}

	val = ROSA_SemaphoreBinaryTake(sem3, ten);
	if (val == true)
	{
		ledOn(LED2_GPIO);
	}
	
	val = ROSA_SemaphoreBinaryRelease (sem3);
	if (val == true)
	{
		ledOff(LED2_GPIO);
	}

	val = ROSA_SemaphoreDelete (sem2);
	if (val == true)
	{
		if (semaphoreHandlerTable[sem2] == NULL)
		{
			ledOn(LED3_GPIO);
		}
	}

	ledOff(LED3_GPIO);


	// IPCP

	semIPCP1 = ROSA_SemaphoreIPCPCreate ();
	semIPCP2 = ROSA_SemaphoreIPCPCreate ();
	semIPCP3 = ROSA_SemaphoreIPCPCreate ();

	if (semIPCP1 != -1)
	{
		ledOn(LED0_GPIO);
	}
	if (semIPCP2 != -1)
	{
		ledOn(LED1_GPIO);
	}
	if (semIPCP3 != -1)
	{
		ledOn(LED2_GPIO);
	}





	val = ROSA_SemaphoreIPCPTake(semIPCP1, nowaiting);
	if (val == true)
	{
		ledOn(LED0_GPIO);
	}
	val = ROSA_SemaphoreIPCPTake(semIPCP2, ten);
	if (val == true)
	{
		ledOn(LED1_GPIO);
	}

	val = ROSA_SemaphoreIPCPTake(semIPCP2, forever);
	if (val == false)
	{
		ledToggle(LED1_GPIO);
	}

	val = ROSA_SemaphoreIPCPRelease(semIPCP2);
	if (val == true)
	{
		ledOff(LED1_GPIO);
	}

	// SHOULD SEMIPCP2 be Taken now by the task wanting to wait forever? Or how does it work?!


	val = ROSA_SemaphoreIPCPRelease(semIPCP1);
	if (val == true)
	{
		ledOff(LED0_GPIO);
	}

	val = ROSA_SemaphoreIPCPTake(semIPCP3, forever);
	if (val == true)
	{
		ledOn(LED2_GPIO);
	}
	val = ROSA_SemaphoreBinaryRelease (semIPCP3);
	if (val == true)
	{
		ledOff(LED2_GPIO);
	}

	val = ROSA_SemaphoreDelete (semIPCP1);
	if (val == true)
	{
		if (semaphoreHandlerTable[semIPCP1] == NULL)
		{
			ledOn(LED3_GPIO);
		}
	}
	
	ledOff(LED3_GPIO);
	
	
***********************************************************/
	
	while(1);
}
