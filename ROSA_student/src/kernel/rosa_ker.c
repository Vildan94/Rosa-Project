#include "kernel/rosa_def.h"
#include "kernel/rosa_ext.h"
#include "kernel/rosa_ker.h"
#include "kernel/rosa_tim.h"
#include "kernel/rosa_scheduler.h"

//Driver includes
#include "drivers/button.h"
#include "drivers/led.h"
#include "drivers/pot.h"
#include "drivers/usart.h"
#include "stdlib.h"

#define MAX 21
tcb * TaskHandleID[MAX]={NULL}; // Initialize all array's fields to NULL

bool ROSA_TaskResume(int HandleId);

void ROSA_init(void)
{
	//Do initialization of I/O drivers
	ledInit();									//LEDs
	buttonInit();								//Buttons
	joystickInit();								//Joystick
	potInit();									//Potentiometer
	usartInit(USART, &usart_options, FOSC0);	//Serial communication
	//TCBLIST = NULL;
	EXECTASK = NULL;
}
 
void ROSA_tcbCreate(tcb * tcbTask, char tcbName[NAMESIZE], void *tcbFunction, int * tcbStack, int tcbStackSize)
{
	int i;

	//Initialize the tcb with the correct values
	for(i = 0; i < NAMESIZE; i++) {
		//Copy the id/name
		tcbTask->id[i] = tcbName[i];
	}

	//Dont link this TCB anywhere yet.
	tcbTask->nexttcb = NULL;

	//Set the task function start and return address.
	tcbTask->staddr = tcbFunction;
	tcbTask->retaddr = (int)tcbFunction;

	//Set up the stack.
	tcbTask->datasize = tcbStackSize;
	tcbTask->dataarea = tcbStack + tcbStackSize;
	tcbTask->saveusp = tcbTask->dataarea;

	//Set the initial SR.
	tcbTask->savesr = ROSA_INITIALSR;

	//Initialize context.
	contextInit(tcbTask);
}

int ROSA_TaskCreate (char ID[NAMESIZE], void *functionPtr, int stackSize, int priority){
	tcb *new_tcb = NULL; int i, j=1;
    int stackData[stackSize];	
	
	if ((!priority < 0) && (!priority >= 20)) {											    // priority must be between 0 to 21 not including these two numbers								
		
		new_tcb = (tcb*)malloc(sizeof(tcb));
		if (new_tcb == NULL){
			// Memory is not allocated
			return -1;
			
		}
		for (i=0; i < NAMESIZE; i++){												     	// The task id/name created for debugging purposes
			new_tcb ->id[i] = ID[i];
		}
		new_tcb->priority = priority;														// The task priority
		
		new_tcb->nexttcb = NULL;															// Don't link this TCB anywhere yet.
		new_tcb->staddr = functionPtr;														// start address
		
		new_tcb->retaddr = (int)functionPtr;												// return address it must be integer
		new_tcb->datasize = stackSize;														// Size of stack
	    new_tcb->dataarea = stackData + stackSize ;										    // Stack data area
		
		new_tcb->saveusp = new_tcb -> dataarea;												// Current stack position
		new_tcb->savesr = ROSA_INITIALSR;													// Put the initial value of the status register to current status register
															// Pointer to the structure of task (tcb)
		// HANDLE ID
		while (j < MAX){
			// If it is empty																	// Check if some array's field is empty or not
			if (TaskHandleID[j]==NULL){
				 new_tcb->handleID = j;	
				 TaskHandleID[j] = new_tcb;
				 break;
			 }
			else 
				 j++; 
		// WHAT IF all fields are full		 
		}	
		contextInit(new_tcb);																// Initialize context.
		Insert_Ready(new_tcb);																// store task to the ready queue
		return new_tcb->handleID;
	}
	else {
		return -1;																			// Error occurred- not possible to create task
	}
		// INSERT to READY queue
}

bool ROSA_TaskDelete(int HandleId){
	tcb *TEMP= READY;
	int j=1;
	//int temp = 0;                               // for return value
	// If executing task tries to delete itself 
	if (EXECTASK->handleID == HandleId){
		
		while(j < MAX){
			if (j == HandleId){
				TaskHandleID[j] = NULL;
				break;
			}
			else{
				j++;
			}
		}
		free(EXECTASK);			
		scheduler();
		contextRestore();
		return true;
		//temp = 1;	
	}
	// READY
	
	while (TEMP != NULL ){		
		if (TEMP->handleID == HandleId){
			while(j < MAX){
				if (j== HandleId){
				TaskHandleID[j] = NULL;
				break;
				}
				else{
					j++;
				}
			}
			free(TEMP);
			scheduler();
			contextRestore();
			//temp = 1;
			return true;
		}
		TEMP = TEMP->nexttcb;
	}
	// WAITING
	TEMP = WAITING;
	while (TEMP != NULL ){
		if (TEMP->handleID == HandleId){
			while(j < MAX){
				if (j == HandleId){
					TaskHandleID[j] = NULL;
					break;
				}
				else{
					j++;
				}
			}
			free(TEMP);
			scheduler();
			contextRestore();
			//temp = 1;
			return true;
		}
		TEMP = TEMP->nexttcb;
	}
	// Blocked
	TEMP = BLOCKED;
	while (TEMP != NULL ){
		if (TEMP->handleID == HandleId){
			while(j < MAX){
				if (j == HandleId){
					TaskHandleID[j] = NULL;
					break;
				}
				else{
					j++;
				}
			}
			free(TEMP);
			scheduler();
			contextRestore();
			return true;
			//temp = 1;
		}
		TEMP = TEMP->nexttcb;
	}
		// SUSPENDED
		TEMP = SUSPENDED;
		while (TEMP != NULL ){
			if (TEMP->handleID == HandleId){
				while(j < MAX){
					if (j == HandleId){
						TaskHandleID[j] = NULL;
						break;
					}
					else{
						j++;
					}
				}
				free(TEMP);
				scheduler();
				contextRestore();
				return true;
				//temp = 1;
			}
			TEMP = TEMP->nexttcb;
		}
		return false;
}

bool ROSA_TaskSusspend(int HandleId){
	Insert_Supsended(HandleId);
	return true;	
}
bool ROSA_TaskResume(int HandleId){
	Resume_Suspended(HandleId);
	return true;
}

void ROSA_tcbInstall(tcb * tcbTask)
{
	tcb * tcbTmp;

	/* Is this the first tcb installed? */
	if(TCBLIST == NULL) {
		TCBLIST = tcbTask;
		TCBLIST->nexttcb = tcbTask;			//Install the first tcb
		tcbTask->nexttcb = TCBLIST;			//Make the list circular
	}
	else {
		tcbTmp = TCBLIST;					//Find last tcb in the list
		while(tcbTmp->nexttcb != TCBLIST) {
			tcbTmp = tcbTmp->nexttcb;
		}
		tcbTmp->nexttcb = tcbTask;			//Install tcb last in the list
		tcbTask->nexttcb = TCBLIST;			//Make the list circular
	}
}
