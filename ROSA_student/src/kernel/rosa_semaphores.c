#include "kernel/rosa_sem.h"
#define MAX_SEM 50
/***********************************************************
 * Create global array needed for storing pointers to semaphores
 ***********************************************************/
semaphoreHandler semaphoreHandlerTable[MAX_SEM] = {NULL};

// OBSERVE SCHEDULUREE OR TASK SHOULD PUT THE EXEC-TASK IN WAITING STATE WHEN RETURN FALSE OR NO?
// SHOULD I DO IT HERE? I MEAN SOMETIMES I DO BLOCK AND THEN RETURN FALSE HOW THE SCHEDULAR NOW WHEN TO NOT AND WHEN TO DO?

/***********************************************************
 handleID ROSA SemaphoreBinaryCreate()
 ***********************************************************/
handleID ROSA_SemaphoreBinaryCreate() {
	
	handleID ID;
	int emptySlot, i;
	semaphoreHandler Handler;
	Handler = NULL;
	Handler = (semaphoreHandler)malloc(sizeof(Semaphore));
	
	if (Handler == NULL) // Error occurred, no memory allocation
	{	
		return -1;
	}

	for(i=0; i<MAX_SEM; i++) // Iterate the array and check for an empty slot
	{
		if (semaphoreHandlerTable[i] == NULL) // Found an empty slot
		{
			emptySlot = i;
			break;
		}
		else // Table is full
		{
			return -1;
		}
	}
	
	interruptDisable();	// Disable interrupt when accessing global variables
	semaphoreHandlerTable[emptySlot] = Handler; // Store the address of the semaphore in the empty slot
	// When a semaphore is deleted the corresponding pointer will point to NULL
	
	Handler->ID = emptySlot; // The ID will be the index of the empty slot
	Handler->isBinary = 1;	// It is a binary semaphore
	Handler->state = Free;		// Semaphore is Free
	Handler->ceil = -1;		// Unused field set to -1
	Handler->taskPriority = -1;	// Unused field set to -1
	
	ID = Handler->ID;
	interruptEnable(); // Enable interrupt when done with accessing global variables
	
	return ID; // Return ID to the user
}

/***********************************************************
 bool ROSA SemaphoreBinaryTake (handleID ID, timerTick ticksToWait)
 ***********************************************************/
bool ROSA_SemaphoreBinaryTake(handleID ID, timerTick ticksToWait)
{ 
	
	if (semaphoreHandlerTable == NULL) // The table does not exist
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID] == NULL)	// If semaphore handler does not exist
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID]->isBinary == 0)	// If semaphore is IPCP
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID]->state == Free) // If the semaphore is free
	{
		interruptDisable();	// Disable interrupt when accessing global variables
		
		semaphoreHandlerTable[ID]->state = Taken;	// Change the state to 1, meaning it is taken now
			
		interruptEnable();	// Enable interrupt
		
		return true;
	}
	else // The semaphore is taken
	{
		if(ticksToWait == 0)	// Semaphore is taken but we do not want to wait
		{
			return false;
		}
		if(ticksToWait == TIMERTICK_MAXVAL)	// Semaphore is taken and we want to wait forever
		{
			EXECTASK->waitSemaphore = TIMERTICK_MAXVAL;
			Insert_Blocked(EXECTASK);	// Task must be blocked
			// WHAT HAPPENS WITH THE TASK?
			return false;	
		}
		
		// Delay the function
		TimerTick newWakeTime;
		TimerTick timeUntilOverflow;
		
		//CRITICAL SECTION
		timeUntilOverflow = TIMERTICK_MAXVAL - SystemTime;
		
		if(ticksToWait > timeUntilOverflow)
		{
			newWakeTime = ticksToWait - timeUntilOverflow;
		}
		else
		{
			newWakeTime = SystemTime + ticksToWait;	// Add delay-length to current time
		}
		//END OF CRITICAL SECTION
		
		EXECTASK->waitUntil = newWakeTime;	// Save wake time into task's attribute
		
		Insert_Blocked(EXECTASK);	// Put the task into the Blocking Queue
		
		ROSA_yield();	// Call the scheduler (ex: yield)

		if(semaphoreHandlerTable[ID]->state == Taken) // Check to see whether the semaphore is still taken
		{
			return false;	// Semaphore is taken, even after waiting
		}
		else if(semaphoreHandlerTable[ID]->state == Free)
		{
			return true;	// Semaphore got free while waiting
		}
	}

}

/***********************************************************
 bool ROSA SemaphoreBinaryRelease (handleID ID)
 ***********************************************************/
 bool ROSA_SemaphoreBinaryRelease (handleID ID)
{
	if (semaphoreHandlerTable == NULL)	// The table does not exist
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID] == NULL) // If semaphore handler does not exist 
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID]->isBinary == 0) // Semaphore is IPCP
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID]->state == Taken)	// If semaphore handler is taken then make it free
	{
		interruptDisable();
		
		semaphoreHandlerTable[ID]->state = Free;	// Change the state to free
		
		interruptEnable();
		
		return true;
	}
	else if(semaphoreHandlerTable[ID]->state == Free) // Semaphore handler is already free
	{
		return true;
	}
	
}

/***********************************************************
 bool ROSA SemaphoreDelete (handleID ID)
 ***********************************************************/
bool ROSA_SemaphoreDelete (handleID ID)
{
	
	if (semaphoreHandlerTable == NULL)	// The table does not exist
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID] == NULL) // If semaphore handler does not exist
	{
		return false;
	}
	
	semaphoreHandler Handler;	// Create a pointer to store the address of the to-be-deleted handler
	
	Handler = semaphoreHandlerTable[ID]; // Get the address 
	
	free(Handler);	// Free it
	
	interruptDisable(); // Disable interrupt when accessing global variables

	semaphoreHandlerTable[ID] = NULL;	// Set table to point to NULL again
	
	interruptEnable();
	
	return true;
}

/***********************************************************
 void ROSA SemaphoreCeil (handleID ID, tcb *tcbtask)
 ***********************************************************/
 void ROSA_SemaphoreCeil (handleID ID, tcb *tcbtask)
 {
	if (semaphoreHandlerTable == NULL)	// The table does not exist
	{	 
		return;
	}
	else if (semaphoreHandlerTable[ID] == NULL) // If semaphore handler does not exist
	{
		return;
	}
	else if (semaphoreHandlerTable[ID]->isBinary == 1)	// If it is a binary semaphore
	{
		return;	
	} 
	else if (tcbtask == NULL)	// Task does not exist
	{
		return;
	}
	else if (tcbtask->priority > semaphoreHandlerTable[ID]->ceil) // Task's priority is higher than current ceil-value
	{
		semaphoreHandlerTable[ID]->ceil = tcbtask->priority; // Update ceil
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
handleID ROSA_SemaphoreIPCPCreate ()
{
	handleID ID;
	int emptySlot, i;
	semaphoreHandler Handler;
	Handler = NULL;
	Handler = (semaphoreHandler)malloc(sizeof(Semaphore));
	
	if (semaphoreHandlerTable == NULL)	// The table does not exist
	{
		return -1;
	}
	else if (Handler == NULL)	// Error occurred, no memory allocation
	{
		return -1;
	}

	for(i=0; i<MAX_SEM; i++)	// Iterate the array and check for an empty slot
	{
		if (semaphoreHandlerTable[i] == NULL)	// Found an empty slot
		{
			emptySlot = i;	
			break;
		}
		else // Table is full
		{
			return -1;
		}
	}
	
	interruptDisable();	// Disable interrupt when accessing global variables
	semaphoreHandlerTable[emptySlot] = Handler;	// Store the address of the semaphore in the empty slot
	// When a semaphore is deleted the corresponding pointer will point to NULL
	
	Handler->ID = emptySlot; // The ID will be the index of the empty slot
	Handler->isBinary = 0;	// It is NOT a binary semaphore
	Handler->state = Free;		// Semaphore is Free
	Handler->ceil = 0;		
	Handler->taskPriority = 0;
	interruptEnable();
	
	ID = Handler->ID;
	return ID;
}

/***********************************************************
 bool ROSA SemaphoreIPCPTake (handleID ID, timerTick ticksToWait)
 ***********************************************************/
bool ROSA_SemaphoreIPCPTake (handleID ID, timerTick ticksToWait)
{
	if (semaphoreHandlerTable == NULL)	// The table does not exist
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID] == NULL)	// If semaphore handler does not exist
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID]->isBinary == 1)	// If semaphore is Binary
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID]->state == Free)	// If the semaphore is free
	{
		// Check if the Task's Prio is greater than all currently locked semaphores 
		int i;
		
		for (i=0; i<MAX_SEM; i++) // Iterate trough the whole array
		{
			if (semaphoreHandlerTable[i] == NULL) // Hop and check next index
			{
				while (semaphoreHandlerTable[i] =! NULL) // Iterate until you find an address
				{
					i++;
				}
			}
			else if (semaphoreHandlerTable[i]->state == Taken) // For all taken semaphores
			{
				if (EXECTASK->priority <= semaphoreHandlerTable[i]->ceil ) // Check if the priority is lower than the ceil value
				{
					// If it is lower then the delay the function
					TimerTick newWakeTime;
					TimerTick timeUntilOverflow;
							
					//CRITICAL SECTION
					timeUntilOverflow = TIMERTICK_MAXVAL - SystemTime;
							
					if(ticksToWait > timeUntilOverflow)
					{
						newWakeTime = ticksToWait - timeUntilOverflow;
					}
					else
					{
						newWakeTime = SystemTime + ticksToWait;	//add delay-length to current time
					}
					//END OF CRITICAL SECTION
							
					EXECTASK->waitUntil = newWakeTime;	//Save wake time into task's attribute
							
					Insert_Blocked(EXECTASK);	//	Put the task into the Block Queue
							
					ROSA_yield();		//Call the scheduler (ex: yield)
					
					// 
					return false;  // WHAT HAPPENS HERE? SHOULD NOT THE SCHEDULAR PUT IT IN BLOCK STATE WHEN GETTING REUTRN FALSE? 
				}
			}
			i++;
		}
		interruptDisable();	// Disable interrupt when accessing global variables
		
		semaphoreHandlerTable[ID]->state = Taken;	// Change the state since it is taken now
		
		if(EXECTASK->priority < semaphoreHandlerTable[ID]->ceil) // If task's priority is lower then the ceil value of the acquired semaphore
		{
			semaphoreHandlerTable[ID]->taskPriority = EXECTASK->priority; // Save task's prio
			EXECTASK->priority = semaphoreHandlerTable[ID]->ceil; // Raise the priority
		}
		
		// Enable interrupt
		interruptEnable();
		
		return true;
	}
	
	// The semaphore is taken
	else
	{
		if(ticksToWait == 0) // Semaphore is taken but we do not want to wait
		{
			return false;
		}
		if(ticksToWait == TIMERTICK_MAXVAL)	// Semaphore is taken and we want to wait forever
		{
			EXECTASK->waitSemaphore = TIMERTICK_MAXVAL;
			Insert_Blocked(EXECTASK);	// Task must be blocked
			// WHAT HAPPENS WITH THE TASK?
			return false;	
		}
		
		// Delay the function
		TimerTick newWakeTime;
		TimerTick timeUntilOverflow;

		//CRITICAL SECTION
		timeUntilOverflow = TIMERTICK_MAXVAL - SystemTime;

		if(ticksToWait > timeUntilOverflow)
		{
			newWakeTime = ticksToWait - timeUntilOverflow;
		}
		else
		{
			newWakeTime = SystemTime + ticksToWait;	// Add delay-length to current time
		}
		//END OF CRITICAL SECTION

		EXECTASK->waitUntil = newWakeTime;	// Save wake time into task's attribute

		Insert_Blocked(EXECTASK);	// Put the task into the Blocking Queue

		ROSA_yield();	// Call the scheduler (ex: yield)
		
		// IS THIS CORRECT? WILL IT WAIT UNTIL THE NEW WAKETIME TO CHECK THIS?
		if(semaphoreHandlerTable[ID]->state == Taken) // Check to see whether the semaphore is still taken
		{
			return false;	// Semaphore is taken, even after waiting
		}
		else if(semaphoreHandlerTable[ID]->state == Free)	// Semaphore got free while waiting
		{
			return true;
		}
	}
}

/***********************************************************
 bool ROSA SemaphoreIPCPRelease (handleID ID)
 ***********************************************************/
bool ROSA_SemaphoreIPCPRelease (handleID ID)
{
	
	if (semaphoreHandlerTable == NULL)	// The table does not exist
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID] == NULL) // If semaphore handler does not exist
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID]->isBinary == 1) // Semaphore is binary
	{
		return false;
	}
	else if (semaphoreHandlerTable[ID]->state == Taken)	// If semaphore handler is taken then make it free
	{
		interruptDisable();  // CRITICAL SECTION
		
		EXECTASK->priority = semaphoreHandlerTable[ID]->taskPriority; // But first change back to old priority 
		semaphoreHandlerTable[ID]->taskPriority = 0; // Reset
		
		semaphoreHandlerTable[ID]->state = Free;	// Change the state to free
		
		interruptEnable();	// END CRITICAL SECTION
		
		return true;
	}
	else if(semaphoreHandlerTable[ID]->state == Free) // Semaphore handler is already free
	{
		return true;
	}
	else
	{
		return false;
	}
	
}
