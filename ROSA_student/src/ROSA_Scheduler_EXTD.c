#include "ROSA_Scheduler_EXTD.h"
#include "rosa_int.h"
#include "rosa_ker.h"
#include "stdlib.h"

bool ChangeIdleTaskPriorityToHighest(void);
bool ChangeIdleTaskPriorityToLowest(void);
void Scheduler(void);

tcb *TaskHandleID[21];//Array for the HandleIDs used by Task Create function when returning ID
// Index represents the ID!!!
extern tcb *EXECTASK=NULL;//Currently executing task
extern tcb *READY=NULL;
extern tcb *BLOCKED=NULL;
extern tcb *WAITING=NULL;
extern tcb *SUSPENDED=NULL;
extern tcb Idle_tcb;
TaskHandleID[0]=&Idle_tcb;

#define TIMERTICK_MAXVAL ULLONG_MAX
#define IDLE_STACK_SIZE 0x40
#define IDLE_PRIORITY 0
#define HIGHEST_PRIORITY 21

void Idle_Task(void){
	while(1){	
	}
}


//CHANGE THE ROSA_YEILD OR MOVE THE CHEDULER TO ORIGINAL SCHEDULER!!!


bool ROSA_Scheduler(void){
	Idle_tcb.handleID = ROSA_TaskCreate("Idle", &Idle_Task, IDLE_STACK_SIZE,IDLE_PRIORITY);//Idle TASK 
	interruptDisable(void);//ROSA function
	Scheduler();
	interruptEnable(void);//ROSA function	
	return true;
}
bool ROSA_SchedulerSuspend(void){	
	if(ChangeIdleTaskPriorityToHighest()){
		ROSA_yield();
		return true;
	}
	else
		return false;
}
bool ROSA_SchedulerResume(void){
	if(ChangeIdleTaskPriorityToLowest()){
		ROSA_yield();
		return true;
	}
	else
		return false;
}

bool ChangeIdleTaskPriorityToHighest(void){//WONT WORK HOW DO YOU GET OUT FROM TASK?!
	TaskHandleID[Idle_tcb.handleID]->priority=HIGHEST_PRIORITY;
	return true;
}
bool ChangeIdleTaskPriorityToLowest(void){
	TaskHandleID[Idle_tcb.handleID]->priority=IDLE_PRIORITY;
	return true;
}


//this should be the original scheduler function 
void Scheduler(void){//When calling Delay it puts a task in waiting queue same for other tasks
	//Suspend task, resume task, semaphore block etc... 
	interruptDisable();
	//All functions take appropriate task from queue and move it in the Ready queue in a sorted manner
	Check_Blocked_Queue(); //Put it in arranged order
	Check_Waiting_Queue();
	Chose_A_Task_Frome_Ready_Queue();//If a task is the same prio it goes at the end of the tasks with same prio's
	//This is done for Round Robin scheduling
	//Context RESTORE and SAVE should be changed in accordance with the new TCB struct.
	interruptEnable();
}


void Check_Blocked_Queue(){
}

void Insert_Blocked(){
	
}

void Resume_Suspended(int handleID){//it needs return type (FALSE) if user tries to resume a task that isn't suspended
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

void Check_Waiting_Queue(){
	tcb *Temp=WAITING;
	if(WAITING==NULL)
	return;
	if(Temp->waitUntil>SystemTime) //If tasks waiting time is bigger than systems time then no task will migrate to ready queue
	return;
	while(Temp!=NULL && Temp->waitUntil<=SystemTime){//If task EXSISTS and its waiting time is lower or equal then
		Insert_Ready(Temp);//Move it to the ready queue
		WAITING=Temp->nexttcb;//Unchain it from the list (since it is always the first element) and assign ptr of the next task to the WAITING
		Temp=WAITING;//Change the temp node to the beginning of the queue
	}
}

void Chose_A_Task_Frome_Ready_Queue(){
	if(READY!=NULL){
		EXECTASK=READY;//Executing ptr to the first node in ready queue
		READY=READY->nexttcb;//Queue should now point to the second node (UNLINK THE FIRST ELEMENT)
		EXECTASK->nexttcb=NULL;//Pointer in the tcb structure shouldn't point nowhere =NULL
	}
}	

void Insert_Waiting(tcb *TASK){
	int waitUntil=TASK->waitUntil;
	tcb *Temp=WAITING;
	if(Temp->waitUntil>waitUntil){//Inserting first task with highest priority in the whole ready queue
		TASK->nexttcb=Temp;
		WAITING=TASK;
		return;
	}
	while(Temp->nexttcb!=NULL){
		if(Temp->nexttcb->waitUntil>waitUntil){//Inserting not first
			TASK->nexttcb=Temp->nexttcb;
			Temp->nexttcb=TASK;
			return;
		}
		Temp=Temp->nexttcb;
	}
	Temp->nexttcb=TASK;
	TASK->nexttcb=NULL;
	return;
}	

void Insert_Ready(tcb *TASK){
	int prio=TASK->priority;
	tcb *Temp=READY;
	if(TASK->priority==IDLE_PRIORITY){//Inserteing the IDLE Task
		while(Temp->nexttcb!=NULL){
			Temp=Temp->nexttcb;
		}
		Temp->nexttcb=TASK;
		TASK->nexttcb=NULL;	
		return;			
	}
	if(Temp->priority<prio){//Inserting first task with highest priority in the whole ready queue
		TASK->nexttcb=Temp;
		READY=TASK;
		return;
	}
	while(Temp->nexttcb!=NULL){
		if(Temp->nexttcb->priority<prio){//Inserting other tasks (not lowest not highest)
			TASK->nexttcb=Temp->nexttcb;
			Temp->nexttcb=TASK;
			return;
		}
		Temp=Temp->nexttcb;
	}
}