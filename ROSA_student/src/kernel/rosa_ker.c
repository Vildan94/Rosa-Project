#include "kernel/rosa_def.h"
#include "kernel/rosa_ext.h"
#include "kernel/rosa_ker.h"
#include "kernel/rosa_tim.h"
#include "kernel/rosa_scheduler.h"
#include "drivers/button.h"
#include "drivers/led.h"
#include "drivers/pot.h"
#include "drivers/usart.h"
#include "stdlib.h"
#include "stdbool.h"

#define MAX 21
#define IDLE_STACK_SIZE 0x40

//Array for the HandleIDs used by Task Create function when returning ID
// Index represents the ID!!!
tcb * TaskHandleID[MAX]={NULL}; // Initialize all array's fields to NULL
tcb *READY;
tcb *BLOCKED;
tcb *WAITING;
tcb *SUSPENDED;
tcb * EXECTASK;
tcb *TCBLIST;
tcb *new_tcb = NULL;
int *stackData;

void Idle_Task(void){
	while(1){
	}
}
void ROSA_init(void)
{
	//Do initialization of I/O drivers
	ledInit();									//LEDs
	buttonInit();								//Buttons
	joystickInit();								//Joystick
	potInit();									//Potentiometer
	usartInit(USART, &usart_options, FOSC0);	//Serial communication
	TCBLIST=NULL;
	EXECTASK = NULL;
	READY=NULL;
	BLOCKED=NULL;
	WAITING=NULL;
	SUSPENDED=NULL;

	timerInit(TIMER_RESOLUTION);
	interruptInit();
	timerStart();
	ROSA_TaskCreate("IDLE",Idle_Task,IDLE_STACK_SIZE,IDLE_PRIORITY);
}
void ROSA_tcbCreate(tcb * tcbTask, char tcbName[NAMESIZE], void *tcbFunction, int * tcbStack, int tcbStackSize,int priority)
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
	
	tcbTask->priority=priority;

	//Initialize context.
	contextInit(tcbTask);
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
//int ROSA_TaskCreate (char ID[NAMESIZE], void *functionPtr, int stackSize, int priority){
	//int i, j=0;	
	//if (priority >= 0 && priority < 20) {											    // priority must be between 0 to 21 not including these two numbers
		//
		//new_tcb = (tcb*)malloc(sizeof(tcb));
		//stackData=malloc(stackSize*sizeof(stackSize));
		//for (i=0; i < NAMESIZE; i++){												     	// The task id/name created for debugging purposes
			//new_tcb ->id[i] = ID[i];
		//}
		//new_tcb->priority = priority;														// The task priority
		//
		//new_tcb->nexttcb = NULL;															// Don't link this TCB anywhere yet.
		//new_tcb->staddr = functionPtr;														// start address
		//
		//new_tcb->retaddr = (int)functionPtr;												// return address it must be integer
		//new_tcb->datasize = stackSize;														// Size of stack
		//new_tcb->dataarea = stackData + stackSize ;										    // Stack data area
		//
		//new_tcb->saveusp = new_tcb -> dataarea;												// Current stack position
		//new_tcb->savesr = ROSA_INITIALSR;
		//new_tcb->waitSemaphore=0;
		//new_tcb->waitUntil=0;
													//// Put the initial value of the status register to current status register
		//// Pointer to the structure of task (tcb)
		////// HANDLE ID
		//while (j < MAX){
			//// If it is empty																	// Check if some array's field is empty or not
			//if (TaskHandleID[j]==NULL){
				//new_tcb->handleID = j;
				//TaskHandleID[j] = new_tcb;
				//break;
			//}
			//else
			//j++;
			//// WHAT IF all fields are full
		//}
		//contextInit(new_tcb);																// Initialize context.
		//Insert_Ready(new_tcb);																// store task to the ready queue
		//return new_tcb->handleID;
	//}
	//else {
		//return -1;																			// Error occurred- not possible to create task
	//}
//}
bool ROSA_TaskSusspend(int HandleId){
	Insert_Supsended(HandleId);
	return true;
}
bool ROSA_TaskResume(int HandleId){
	Resume_Suspended(HandleId);
	return true;
}
int ROSA_TaskCreate (char ID[NAMESIZE], void *functionPtr, int stackSize, int priority){
	int i, j=1;
	
	if(priority >= 0 && priority <= 20 ){
		if(ID == "IDLE"){																		// If task is IDLE
			new_tcb->handleID = 0;																// Handle ID will be 0
			TaskHandleID[j] = new_tcb;
			
			new_tcb = (tcb*)malloc(sizeof(tcb));
			stackData=malloc(stackSize*sizeof(stackSize));
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
			new_tcb->savesr = ROSA_INITIALSR;
			new_tcb->waitSemaphore=0;
			new_tcb->waitUntil=0;
			contextInit(new_tcb);																// Initialize context.
			Insert_Ready(new_tcb);																// store task to the ready queue
			return new_tcb->handleID;
		}
		
		new_tcb = (tcb*)malloc(sizeof(tcb));
		stackData=malloc(stackSize*sizeof(stackSize));
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
		new_tcb->savesr = ROSA_INITIALSR;
		new_tcb->waitSemaphore=0;
		new_tcb->waitUntil=0;
		
		while (j < MAX){																	// Check if some array's field is empty or not
			if (TaskHandleID[j]==NULL){
				new_tcb->handleID = j;
				TaskHandleID[j] = new_tcb;
				break;
			}
			else
			j++;
		}
		contextInit(new_tcb);																	// Initialize context.
		Insert_Ready(new_tcb);																	// store task to the ready queue
		return new_tcb->handleID;
	}
	else {
		return -1;																				// Error occurred- not possible to create task
	}
	
}
bool ROSA_TaskDelete(int HandleId){
	tcb *TEMP= READY; 
	// If executing task tries to delete itself
	if (EXECTASK->handleID == HandleId){		
		TaskHandleID[HandleId] = NULL;
		free(EXECTASK);
		ROSA_yield();
		return true;
	}
	//// READY	
	//while (TEMP != NULL ){
		//if (TEMP->handleID == HandleId){
			//TaskHandleID[HandleId] = NULL;
			//free(TEMP);
			//ROSA_yield();
			//return true;
		//}
		//TEMP = TEMP->nexttcb;
	//}
	////// WAITING
	//tcb *PREVIOUS = WAITING; 
	//TEMP = WAITING;
	//while (TEMP != NULL ){
		//if (TEMP->handleID == HandleId){
			//TaskHandleID[HandleId] = NULL;
			//PREVIOUS->nexttcb = TEMP->nexttcb;
			//free(TEMP);
			//TEMP->nexttcb = NULL;
			//TEMP = NULL;
			//ROSA_yield();
			//return true;
		//}
		//PREVIOUS = TEMP;
		//TEMP = TEMP->nexttcb;
	//}
	//////// Blocked
	//////TEMP = BLOCKED;
	//////while (TEMP != NULL ){
		//////if (TEMP->handleID == HandleId){
			//////while(j < MAX){
				//////if (j == HandleId){
				//////TaskHandleID[HandleId] = NULL;
					//////break;
				//////}
				//////else{
					//////j++;
				//////}
			//////}
			//////free(TEMP);
			//////scheduler();
			//////contextRestore();
			//////return true;
			////////temp = 1;
		//////}
		//////TEMP = TEMP->nexttcb;
	//////}
	////// SUSPENDED
	//TEMP = SUSPENDED;
	//while (TEMP != NULL ){
		//if (TEMP->handleID == HandleId){
			//TaskHandleID[HandleId] = NULL;
			//free(TEMP);
			//ROSA_yield();
			//return true;
		//}
		//TEMP = TEMP->nexttcb;
	//}
}

