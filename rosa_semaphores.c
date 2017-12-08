//Kernel includes
#include "kernel/rosa_ker.h"
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
	
	if (binarySemaphoreHandler == NULL)
	{
		// Error occured, no memory allocation
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
	
	// Store the address of the semaphore in the empty slot
	// When a semaphore is deleted the corresponding pointer will point to NULL
	
	semaphoreHandlerTable[emptySlot] = binarySemaphoreHandler;
	
	// The ID will be the index of the empty slot
	
	binarySemaphoreHandler->ID = emptySlot;
	binarySemaphoreHandler->state = 0;
	ID = binarySemaphoreHandler->ID;
	
	return ID;
}

/***********************************************************
 bool ROSA SemaphoreBinaryTake (handleID ID, timerTick ticksToWait)
 ***********************************************************/

bool ROSA SemaphoreBinaryTake(handleID ID, timerTick ticksToWait)
{ 
	
	
	Get current time stamp
	
	
	If hashtable->key[handleID].pointer->state is equal to 1 an
	Then execute
	hashtable->key[handleID].pointer->state is equal to 0
	Return true
	Else
	Create delay until functionality here but without the task yield
	function and send to block state
	If Return false
}

/***********************************************************
 bool ROSA SemaphoreBinaryRelease (handleID ID)
 ***********************************************************/

/***********************************************************
 bool ROSA SemaphoreDelete (handleID ID)
 ***********************************************************/

/***********************************************************
 void ROSA SemaphoreCeil (handleID ID, tcb *tcbtask)
 ***********************************************************/

/***********************************************************
 handleID ROSA SemaphoreIPCPCreate ()
 ***********************************************************/

/***********************************************************
 bool ROSA SemaphoreIPCPTake (handleID ID, timerTick ticksToWait)
 ***********************************************************/

/***********************************************************
 bool ROSA SemaphoreIPCPRelease (handleID ID))
 ***********************************************************/

