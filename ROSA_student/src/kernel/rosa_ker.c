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

#define MAX_NR_TASK 21
#define IDLE_STACK_SIZE 0x40


//Array for the HandleIDs used by Task Create function when returning ID
// Index represents the ID!!!
tcb * TaskHandleID[MAX_NR_TASK]={NULL}; // Initialize all array's fields to NULL
tcb *READY;
tcb *BLOCKED;
tcb *WAITING;
tcb *SUSPENDED;
tcb * EXECTASK;
tcb *TCBLIST;


bool Search_Queue(tcb *Queue,int HandleId);

void Idle_Task(void){
	while(1){
		if (TaskHandleID[0]->priority==HIGHEST_PRIORITY){
			TimerTick FirstWake=ROSA_getTickTime();
			while(1){
				if(SystemTime>FirstWake+3600000)
				break;
				if(isButton(PUSH_BUTTON_0)==0)
				break;
			}
		}	
		
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
	Create_Idle();
}
void Create_Idle(){
	tcb *new_tcb = NULL;							// If task is IDLE
	int *stackData=NULL;
	new_tcb = (tcb*)malloc(sizeof(tcb));
	if(new_tcb==NULL)
	return -1;
		
	stackData=malloc(IDLE_STACK_SIZE*sizeof(int));
	if (stackData==NULL){
		free(new_tcb);
		return -1;
	}
			
	new_tcb->HandleID = 0;																// Handle ID will be 0
	TaskHandleID[0] = new_tcb;												     	// The task id/name created for debugging purposes
	new_tcb ->id[0] = "I";
	
	new_tcb->priority = 0;														// The task priority
				
	new_tcb->nexttcb = NULL;															// Don't link this TCB anywhere yet.
	new_tcb->staddr = *Idle_Task;														// start address
				
	new_tcb->retaddr = (int)Idle_Task;												// return address it must be integer
	new_tcb->datasize = IDLE_STACK_SIZE;														// Size of stack
	new_tcb->dataarea = stackData + IDLE_STACK_SIZE ;										    // Stack data area
				
	new_tcb->saveusp = new_tcb -> dataarea;												// Current stack position
	new_tcb->savesr = ROSA_INITIALSR;
	new_tcb->waitSemaphore=0;
	new_tcb->waitUntil=0;
	contextInit(new_tcb);																// Initialize context.
	Insert_Ready(new_tcb);																// store task to the ready queue
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
	tcb *new_tcb = NULL;
	int *stackData=NULL;
	
	if(priority <= 0 || priority > 20 )	//if invalid prio, exit
		return -1;
		
	if(stackSize<=0)
		return -1;
							
	new_tcb = (tcb*)malloc(sizeof(tcb));
	if(new_tcb==NULL)
		return -1;
	
	stackData = malloc(stackSize*sizeof(int));
	if(stackData==NULL){
		free(new_tcb);
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
	new_tcb->savesr = ROSA_INITIALSR;
	new_tcb->waitSemaphore=0;
	new_tcb->waitUntil=0;
		
	while (j < MAX_NR_TASK){																	// Check if some array's field is empty or not
		if (TaskHandleID[j]==NULL){
			new_tcb->HandleID = j;
			TaskHandleID[j] = new_tcb;
			break;
		}
		else
		j++;
	}
	if(j==MAX_NR_TASK)
	return -1;
	
	contextInit(new_tcb);																	// Initialize context.
	Insert_Ready(new_tcb);																	// store task to the ready queue
	return new_tcb->HandleID;	
}

bool ROSA_TaskDelete(int HandleId){	
	if(HandleId<1 || HandleId>20)
	return false;	
	
	if(TaskHandleID[HandleId]==NULL)
	return true;
	
	// If executing task tries to delete itself
	if (EXECTASK->HandleID == HandleId ){
		interruptDisable();
		TaskHandleID[HandleId] = NULL;				// free space in array handle ID
		free(EXECTASK->dataarea-EXECTASK->datasize);				    // deallocate stack
		free(EXECTASK);								// deallocate task structure
		interruptEnable();
		ROSA_yield();								// it takes next task from ready queue
		return true;
	}
	if(Search_Queue(WAITING, HandleId))
		return true;
	if(Search_Queue(READY, HandleId))
		return true;
	if(Search_Queue(BLOCKED, HandleId))
		return true;
	if(Search_Queue(SUSPENDED, HandleId))
		return true;

}

bool Search_Queue(tcb *Queue,int HandleId){
	tcb *PREVIOUS, *TEMP;
	TEMP = Queue;
	PREVIOUS = Queue;
	if(TEMP!=NULL && TEMP->HandleID==HandleId){
		Queue=TEMP->nexttcb;
 		//TaskHandleID[HandleId] = NULL;
 		//free(EXECTASK->dataarea-EXECTASK->datasize);
 		//free(TEMP);
		return true;
	}
	while (TEMP != NULL ){
		if (TEMP->HandleID == HandleId){
			PREVIOUS->nexttcb = TEMP->nexttcb;   // OK
			interruptDisable();
			TaskHandleID[HandleId] = NULL;
			free(TEMP->dataarea-TEMP->datasize);	
			free(TEMP);
			interruptEnable();
			return true;
		}
		PREVIOUS = TEMP;
		TEMP = TEMP->nexttcb;
	}
	return false;
}

