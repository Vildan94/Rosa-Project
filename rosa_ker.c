/*****************************************************************************

                 ///////,   .////    .///' /////,
                ///' ./// ///'///  ///,     '///
               ///////'  ///,///   '/// //;';//,
             ,///' ////,'/////',/////'  /////'/;,

    Copyright 2010 Marcus Jansson <mjansson256@yahoo.se>

    This file is part of ROSA - Realtime Operating System for AVR32.

    ROSA is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ROSA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ROSA.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
/* Tab size: 4 */

//Kernel includes
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
tcb * handleID[MAX]={NULL}; // Initialize all array's fields to NULL
//int counter = 1;
//handleID * array;

/***********************************************************
 * TCBLIST
 *
 * Comment:
 * 	Global variables that contain the list of TCB's that
 * 	have been installed into the kernel with ROSA_tcbInstall()
 **********************************************************/
tcb * TCBLIST;

/***********************************************************
 * EXECTASK
 *
 * Comment:
 * 	Global variables that contain the current running TCB and other queues
 **********************************************************/
tcb * EXECTASK;
tcb * READY=NULL;
tcb * BLOCKED=NULL;
tcb * WAITING=NULL;
tcb * SUSPENDED=NULL;

/***********************************************************
 * ROSA_init
 *
 * Comment:
 * 	Initialize the ROSA system
 *
 **********************************************************/
void ROSA_init(void)
{
	//Do initialization of I/O drivers
	ledInit();									//LEDs
	buttonInit();								//Buttons
	joystickInit();								//Joystick
	potInit();									//Potentiometer
	usartInit(USART, &usart_options, FOSC0);	//Serial communication

	//Start with empty TCBLIST and no EXECTASK.
	TCBLIST = NULL;
	EXECTASK = NULL;

	//Initialize the timer to 100 ms period.
	//...
	//timerInit(100);
	//...
}

/***********************************************************
 * ROSA_tcbCreate
 *
 * Comment:
 * 	Create the TCB with correct values.
 *
 **********************************************************/
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

	//int createArray(){
	//
	//handleID * array = NULL;
	//array = (handleID *)malloc(( 21 * sizeof(handleID *)));
	//
	//return array;
	//
	//}

//int *array(){
	//
	//handleID * array = NULL;
	//array = (handleID *)malloc ((21 * sizeof(handleID *)));
	//return array;
//}

//int *createArray(){
//
	//int i;
	//for(i = 0; i < max; i++){
		//handleID[i]=NULL;
	//}
	//
//}
int ROSA_TaskCreate (char ID[name_size], void *functionPtr, int stackSize, int priority){
	tcb *new_tcb = NULL; int i, j=1;
    int stackData[stackSize];
	
	
	if ((!priority < 0) && (!priority >= 20)) {											    // priority must be between 0 to 21 not including these two numbers								
		
		new_tcb = (tcb*)malloc(sizeof(tcb));
		if (new_tcb == NULL){
			// Memory is not allocated
			return -1;
			
		}
		for (i=0; i < name_size; i++){												     	// The task id/name created for debugging purposes
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
			if (handleID[j]==NULL){
				 new_tcb->handleID = j;	
				 handleID[j] = new_tcb;
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
	tcb *TEMP = READY; int j=1;
	//int temp = 0;                               // for return value
	// If executing task tries to delete itself 
	if (EXECTASK->handleID == HandleId){
		
		while(j < MAX){
			if (handleID [j] == HandleId){
				handleID[j] = NULL;
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
				if (handleID [j] == HandleId){
				handleID[j] = NULL;
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
				if (handleID [j] == HandleId){
					handleID[j] = NULL;
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
				if (handleID [j] == HandleId){
					handleID[j] = NULL;
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
					if (handleID [j] == HandleId){
						handleID[j] = NULL;
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
		
		// Returning value
//	if (temp = 0) {
//		return false; 
//	}
//	else {
//		return true;
//	}
		
}

bool ROSA_TaskSusspend(int HandleId){
	
	Insert_Supsended(HandleId);
	return true;
	
}
bool ROSA_TaskResume(int HandleId){
	
	Resume_Suspended(HandleId);
	return true;
	
}


/***********************************************************
 * ROSA_tcbInstall
 *
 * Comment:
 * 	Install the TCB into the TCBLIST.
 *
 **********************************************************/
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


void Resume_Suspended(int handleID){
	//It needs return type (FALSE) if user tries to resume a task that isn't suspended
	tcb *Temp=SUSPENDED, *Previous=SUSPENDED;
	while(Temp!=NULL){
		if(Temp->handleID==handleID){
			Previous->nexttcb=Temp->nexttcb;
			Insert_Ready(Temp);
			return;
		}
		Previous=Temp;
		Temp=Temp->nexttcb;
	}
}
void Insert_Supsended(int handleID){//Order of the elements doesn't matter
	tcb *Temp=READY, *Previous=READY;
	
	//EXECUTING MOVE
	if(EXECTASK->handleID==handleID){//the executing task wants to suspend itself
		EXECTASK->nexttcb=SUSPENDED;
		SUSPENDED=EXECTASK;
		ROSA_yield();
	}
	
	//READY SEARCH
	if(Temp!=NULL && Temp->handleID==handleID){//If it is the first element
		READY=Temp->nexttcb;
		Temp->nexttcb=SUSPENDED;
		SUSPENDED=Temp;
	}
	while(Temp->nexttcb!=NULL){//The rest of the elements
		if(Temp->nexttcb->handleID==handleID){
			Temp=Temp->nexttcb;
			Previous->nexttcb=Temp->nexttcb;
			Temp->nexttcb=SUSPENDED;
			SUSPENDED=Temp;
			return;
		}
		Temp=Temp->nexttcb;
	}
	
	//WAITING QUEUE SEARCH
	Temp=WAITING;
	Previous=WAITING;
	if(Temp!=NULL && Temp->handleID==handleID){//If it is the first element
		WAITING=Temp->nexttcb;
		Temp->nexttcb=SUSPENDED;
		SUSPENDED=Temp;
	}
	while(Temp->nexttcb!=NULL){//The rest of the elements
		if(Temp->nexttcb->handleID==handleID){
			Temp=Temp->nexttcb;
			Previous->nexttcb=Temp->nexttcb;
			Temp->nexttcb=SUSPENDED;
			SUSPENDED=Temp;
			return;
		}
		Temp=Temp->nexttcb;
	}
	
	//BLOCKED SEARCH
	Temp=BLOCKED;
	Previous=BLOCKED;
	if(Temp!=NULL && Temp->handleID==handleID){//If it is the first element
		BLOCKED=Temp->nexttcb;
		Temp->nexttcb=SUSPENDED;
		SUSPENDED=Temp;
	}
	while(Temp->nexttcb!=NULL){//The rest of the elements
		if(Temp->nexttcb->handleID==handleID){
			Temp=Temp->nexttcb;
			Previous->nexttcb=Temp->nexttcb;
			Temp->nexttcb=SUSPENDED;
			SUSPENDED=Temp;
			return;
		}
		Temp=Temp->nexttcb;
	}
}