//Kernel includes
#include "kernel/rosa_ker.h"
#include "kernel/rosa_int.h"
//I/O driver includes
#include "drivers/led.h"
#include "drivers/delay.h"
//Include configuration
#include "rosa_config.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "rosa_tim.h"

#define Taken 1
#define Free 0

struct binarySemaphore {
	int ID;
	int state;
	int flag = 0;
};

typedef struct binarySemaphore BinarySemaphore;

struct semaphore {
	int ceil;
	int ID;
	int state;
	int taskPriority;
	int flag = 1;
};

typedef struct semaphore Semaphore;

typedef BinarySemaphore * binarySemaphoreHandler;
typedef Semaphore * semaphoreHandler;

typedef unsigned long long timerTick;

typedef int handleID;

/***********************************************************
 * Create array of pointers needed for storing semaphores
 ***********************************************************/

handleID * createArray()
{
	handleID * array = NULL;
	array = (handleID *)malloc((50 * sizeof(handleID *)));
	
	// Check for errors in the other functions
	// If an error occurred the function will return NULL   
	
	return array;
}


/***********************************************************
 Must be global, must be accessible to other functions
 ***********************************************************/

 handleID * array;
 semaphoreHandlerTable = createArray();
 

/***********************************************************
 handleID ROSA SemaphoreBinaryCreate()
 ***********************************************************/

handleID ROSA SemaphoreBinaryCreate() {
	
	handleID ID;
	int emptySlot;
	binarySemaphoreHandler = malloc(sizeof(binarySemaphore));
	
	if (semaphoreHandlerTable == NULL)
	{
		// The table does not exist
		return -1;
	}
	
	else if (binarySemaphoreHandler == NULL)
	{
		// Error occurred, no memory allocation
		return -1;
	}

	// Iterate the array and check for an empty slot
	
	(for i=0; i<50; i++)
	{
		if (semaphoreHandlerTable[i] == NULL)
		{
			// Found an empty slot
			emptySlot = i;
			break;
		}
		if (semaphoreHandlerTable[i] != NULL)
			// Table is full
			return -1;
	}
	
	
	// Disable interrupt when accessing global variables
	interruptDisable(void);
	// Store the address of the semaphore in the empty slot
	// When a semaphore is deleted the corresponding pointer will point to NULL
	semaphoreHandlerTable[emptySlot] = binarySemaphoreHandler;

	
	// The ID will be the index of the empty slot
	binarySemaphoreHandler->ID = emptySlot;
	binarySemaphoreHandler->state = 0;
	ID = binarySemaphoreHandler->ID;
	interruptEnable(void);
	
	return ID;
}

/***********************************************************
 bool ROSA SemaphoreBinaryTake (handleID ID, timerTick ticksToWait)
 ***********************************************************/

bool ROSA SemaphoreBinaryTake(handleID ID, timerTick ticksToWait)
{ 
	
	if (semaphoreHandlerTable == NULL)
	{
		// The table does not exist
		return false;
	}
	
	// If semaphore handler does not exist
	else if (semaphoreHandlerTable[ID] == NULL)
	{
		return false;
	}
	
	// If semaphore is IPCP
	else if (semaphoreHandlerTable[ID]->flag == 1)
	{
		return false;
	}
	
	// If the semaphore is free
	else if (semaphoreHandlerTable[ID]->state == Free)
	{
		// Disable interrupt when accessing global variables
		interruptDisable(void);
		
		// Change the state to 1, meaning it is taken now
		semaphoreHandlerTable[ID]->state = Taken;
		
		// Enable interrupt
		interruptEnable(void);
		
		return true;
	}
	
	// The semaphore is taken
	else 
	{
		// Semaphore is taken but we do not want to wait
		if(ticksToWait == 0)
		{
			return false;
		}
		
		// Semaphore is taken and we want to wait forever
		if(ticksToWait == TIMERTICK_MAXVAL)
		{
			// Task must be blocked
			// Request the tcb handle ID by executing task
			// Call function to send the task to blocking state
			// Send some kind of information to the scheduler to notify it to send the task to the blocking state
			
		}
		
		// Delay the function
		ROSA_Delay(ticksToWait);
		
		// Check to see whether the semaphore is still taken
		if(semaphoreHandlerTable[ID]->state == Taken)
		{
			// Semaphore is taken, even after waiting
			return false;	
		}
		else(semaphoreHandlerTable[ID]->state == Free)
		{
			// Semaphore got free while waiting
			return true;
		}
		
	}

}

/***********************************************************
 bool ROSA SemaphoreBinaryRelease (handleID ID)
 ***********************************************************/

 bool ROSA SemaphoreBinaryRelease (handleID ID)
{
	
	if (semaphoreHandlerTable == NULL)
	{
		// The table does not exist
		return false;
	}
		
	// If semaphore handler does not exist 
	if (semaphoreHandlerTable[ID] == NULL)
	{
		return false;
	}
	
	// If semaphore handler is taken then make it free
	else if (semaphoreHandlerTable[ID]->state == Taken)
	{
		interruptDisable(void);
		// Change the state to free
		semaphoreHandlerTable[ID]->state = Free;
		
		interruptEnable(void);
		return true;
	}
	
	// If semaphore handler is already free
	else if(semaphoreHandlerTable[ID]->state == Free)
	{
		return true;
	}
	
}

/***********************************************************
 bool ROSA SemaphoreDelete (handleID ID)
 ***********************************************************/

bool ROSA SemaphoreDelete (handleID ID)
{
	
	if (semaphoreHandlerTable == NULL)
	{
		// The table does not exist
		return false;
	}

	// If semaphore handler does not exist
	if (semaphoreHandlerTable[ID] == NULL)
	{
		return false;
	}
	
	// Create a pointer to store the address of the to-be-deleted handler
	binarySemaphoreHandler Handler;
	
	// Get the address 
	Handler = semaphoreHandlerTable[ID];
	
	// Free it
	free(Handler);
	
	interruptDisable(void);
	
	// Set table to point to NULL again
	semaphoreHandlerTable[ID] = NULL;
	
	interruptEnable(void);
	
	return true;
}


/***********************************************************
 void ROSA SemaphoreCeil (handleID ID, tcb *tcbtask)
 ***********************************************************/

 void ROSA SemaphoreCeil (handleID ID, tcb *tcbtask)
 {
	if (semaphoreHandlerTable == NULL)
	{
		 // The table does not exist
		return;
	}
	
	 // If semaphore handler does not exist
	else if (semaphoreHandlerTable[ID] == NULL)
	{
		return;
	}
	 
	else if (semaphoreHandlerTable[ID]->flag == 0)
	{
		// It is a binary semaphore
		return;
	}
		  
	else if (*tcbtask == NULL)
	{
		// Task does not exist
		return;
	}
	
	else if (*tcbtask->priority > semaphoreHandlerTable[ID]->ciel)
	{
		semaphoreHandlerTable[ID]->ciel = *tcbtask->priotity;
		return;
	}
	else
	{
		return;
	}
	 
 }

/***********************************************************
 handleID ROSA SemaphoreIPCPCreate ()
 ***********************************************************/

handleID ROSA SemaphoreIPCPCreate ()
{
	
}

/***********************************************************
 bool ROSA SemaphoreIPCPTake (handleID ID, timerTick ticksToWait)
 ***********************************************************/

bool ROSA SemaphoreIPCPTake (handleID ID, timerTick ticksToWait)
{
	
}

/***********************************************************
 bool ROSA SemaphoreIPCPRelease (handleID ID))
 ***********************************************************/

bool ROSA SemaphoreIPCPRelease (handleID ID))
{
	
}
