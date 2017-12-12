#include "kernel/rosa_sem.h"

/***********************************************************
 * Create global array needed for storing pointers to semaphores
 ***********************************************************/

// OBSERVE SCHEDULUREE OR TASK SHOULD PUT THE EXEC-TASK IN WAITING STATE WHEN RETURN FALSE OR NO?
// SHOULD I DO IT HERE? I MEAN SOMETIMES I DO BLOCK AND THEN RETURN FALSE HOW THE SCHEDULAR NOW WHEN TO NOT AND WHEN TO DO?

/***********************************************************
 handleID ROSA SemaphoreBinaryCreate()
 ***********************************************************/

handleID ROSA_SemaphoreBinaryCreate()
{
	int i;

	for(i=0; i<MAX_SEM; i++) // Iterate the global array and check for an empty slot
	{
		if (semaphoreHandlerTable[i] == NULL) // Found an empty slot
		{
			//
			semaphoreHandlerTable[i] = (semaphoreHandler)malloc(sizeof(Semaphore));
			
			if (semaphoreHandlerTable[i] == NULL)
			{
				return -1;
			}
			
			interruptDisable(); // Disable interrupt when accessing global variables
			semaphoreHandlerTable[i]->ID = i; // The ID will be the index of the empty slot
			semaphoreHandlerTable[i]->isBinary = 1;	// It is a binary semaphore
			semaphoreHandlerTable[i]->state = Free;		// Semaphore is Free
			semaphoreHandlerTable[i]->ceil = -1;		// Unused field set to -1
			semaphoreHandlerTable[i]->taskPriority = -1;	// Unused field set to -1
			interruptEnable(); // Enable interrupt when done with accessing global variables
				
			return i; // Return ID to the user
		}
	}
	
	// After checking the whole table and it is full return
	return -1;
	
	// When a semaphore is deleted the corresponding pointer will point to NULL
}

/***********************************************************
 bool ROSA SemaphoreBinaryTake (handleID ID, timerTick ticksToWait)
 ***********************************************************/
bool ROSA_SemaphoreBinaryTake(handleID ID, timerTick ticksToWait)
{ 
	if (semaphoreHandlerTable == NULL) // The table does not exist
		return false;

	if (semaphoreHandlerTable[ID] == NULL)	// If semaphore handler does not exist
		return false;

	if (semaphoreHandlerTable[ID]->isBinary == 0)	// If semaphore is IPCP
		return false;

	if (semaphoreHandlerTable[ID]->state == Free) // If the semaphore is free
	{
		interruptDisable();	// Disable interrupt when accessing global variables
		
		semaphoreHandlerTable[ID]->state = Taken;	// Change the state to 1, meaning it is taken now
			
		interruptEnable();	// Enable interrupt
		
		return true;
	}
	 
	// The semaphore is taken
	//ToDo: THE EXEC TASK MUST KNOW WHICH SEM IT IS WAITING FOR
	switch(ticksToWait)
	{
		case 0: 
			return false;
			
		case TIMERTICK_MAXVAL: 
			EXECTASK->waitSemaphore = TIMERTICK_MAXVAL;
			//ToDo: THE EXEC TASK MUST KNOW WHICH SEM IT IS WAITING FOR
			
			Insert_Blocked(EXECTASK);	// Task must be blocked
			ROSA_yield();
			break;
			
		default: 
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
			//ToDo: THE EXEC TASK MUST KNOW WHICH SEM IT IS WAITING FOR
			
			Insert_Blocked(EXECTASK);	// Put the task into the Blocking Queue	
			ROSA_yield();	// Call the scheduler (ex: yield)
			
			// THIS WILL NOT HAPPEN! HOW SHOULD WE CHECK THIS AFTER THE WAKETIME?
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
	if (semaphoreHandlerTable[ID] == NULL) // If semaphore handler does not exist 
	{
		return false;
	}
	if (semaphoreHandlerTable[ID]->isBinary == 0) // Semaphore is IPCP
	{
		return false;
	}
	if (semaphoreHandlerTable[ID]->state == Taken)	// If semaphore handler is taken then make it free
	{
		interruptDisable();
		
		semaphoreHandlerTable[ID]->state = Free;	// Change the state to free
		
		interruptEnable();
		
		return true;
	}
	if(semaphoreHandlerTable[ID]->state == Free) // Semaphore handler is already free
	{
		return true;
	}
	
	return false;
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
	if (semaphoreHandlerTable[ID] == NULL) // If semaphore handler does not exist
	{
		return false;
	} 
	
	interruptDisable(); // Disable interrupt when accessing global variables

	free(semaphoreHandlerTable[ID]);	// Free it
	
	semaphoreHandlerTable[ID] = NULL;	// Set table to point to NULL again
	
	interruptEnable(); // Enable interrupt again
	
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
	if (semaphoreHandlerTable[ID] == NULL) // If semaphore handler does not exist
	{
		return;
	}
	if (semaphoreHandlerTable[ID]->isBinary == 1)	// If it is a binary semaphore
	{
		return;	
	} 
	if (tcbtask == NULL)	// Task does not exist
	{
		return;
	}
	if (tcbtask->priority > semaphoreHandlerTable[ID]->ceil) // Task's priority is higher than current ceil-value
	{
		semaphoreHandlerTable[ID]->ceil = tcbtask->priority; // Update ceil
		return;
	}
	return;
 }

/***********************************************************
 handleID ROSA SemaphoreIPCPCreate ()
 ***********************************************************/
handleID ROSA_SemaphoreIPCPCreate ()
{
	int i;

	for(i=0; i<MAX_SEM; i++) // Iterate the global array and check for an empty slot
	{
		if (semaphoreHandlerTable[i] == NULL) // Found an empty slot
		{
			//
			semaphoreHandlerTable[i] = (semaphoreHandler)malloc(sizeof(Semaphore));
				
			if (semaphoreHandlerTable[i] == NULL)
			{
				return -1;
			}
				
			interruptDisable(); // Disable interrupt when accessing global variables
			semaphoreHandlerTable[i]->ID = i; // The ID will be the index of the empty slot
			semaphoreHandlerTable[i]->isBinary = 0;	// It is a binary semaphore
			semaphoreHandlerTable[i]->state = Free;		// Semaphore is Free
			semaphoreHandlerTable[i]->ceil = 0;
			semaphoreHandlerTable[i]->taskPriority = 0;
			interruptEnable(); // Enable interrupt when done with accessing global variables
				
			return i; // Return ID to the user
		}
	}
		
	// After checking the whole table and it is full
	return -1;
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
	if (semaphoreHandlerTable[ID] == NULL)	// If semaphore handler does not exist
	{
		return false;
	}
	if (semaphoreHandlerTable[ID]->isBinary == 1)	// If semaphore is Binary
	{
		return false;
	}
	if (semaphoreHandlerTable[ID]->state == Free)	// If the semaphore is free
	{
		// Check if the Task's priority is greater than all currently locked semaphores 
		int i;
		
		for (i=0; i<MAX_SEM; i++) // Iterate trough the whole array
		{
			if (semaphoreHandlerTable[i] != NULL && semaphoreHandlerTable[i]->state == Taken) // For all taken semaphores
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
					//ToDo: THE EXEC TASK MUST KNOW WHICH SEM IT IS WAITING FOR
							
					Insert_Blocked(EXECTASK);	//	Put the task into the Block Queue
							
					ROSA_yield();		//	Call the scheduler (ex: yield)
					
					// Should not continue from here
				}
			}
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
	//ToDo: THE EXEC TASK MUST KNOW WHICH SEM IT IS WAITING FOR
	switch(ticksToWait)
	{
		case 0: 
			return false;
			
		case TIMERTICK_MAXVAL: 
			EXECTASK->waitSemaphore = TIMERTICK_MAXVAL;
			//ToDo: THE EXEC TASK MUST KNOW WHICH SEM IT IS WAITING FOR
			
			Insert_Blocked(EXECTASK);	// Task must be blocked
			ROSA_yield();
			break;
			
		default: 
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
			//ToDo: THE EXEC TASK MUST KNOW WHICH SEM IT IS WAITING FOR
			
			Insert_Blocked(EXECTASK);	// Put the task into the Blocking Queue	
			ROSA_yield();	// Call the scheduler (ex: yield)
			
			// THIS WILL NOT HAPPEN! HOW SHOULD WE CHECK THIS AFTER THE WAKETIME?
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
 bool ROSA SemaphoreIPCPRelease (handleID ID)
 ***********************************************************/
bool ROSA_SemaphoreIPCPRelease (handleID ID)
{
	if (semaphoreHandlerTable == NULL)	// The table does not exist
	{
		return false;
	}
	if (semaphoreHandlerTable[ID] == NULL) // If semaphore handler does not exist
	{
		return false;
	}
	if (semaphoreHandlerTable[ID]->isBinary == 1) // Semaphore is binary
	{
		return false;
	}
	if (semaphoreHandlerTable[ID]->state == Taken)	// If semaphore handler is taken then make it free
	{
		interruptDisable();  // CRITICAL SECTION
		
		EXECTASK->priority = semaphoreHandlerTable[ID]->taskPriority; // But first change back to old priority 
		semaphoreHandlerTable[ID]->taskPriority = 0; // Reset
		
		semaphoreHandlerTable[ID]->state = Free;	// Change the state to free
		
		interruptEnable();	// END CRITICAL SECTION
		
		return true;
	}
	if(semaphoreHandlerTable[ID]->state == Free) // Semaphore handler is already free
	{
		return true;
	}
	
	return false;
	
}
