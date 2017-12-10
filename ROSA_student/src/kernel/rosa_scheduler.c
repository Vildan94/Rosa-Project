#include "kernel/rosa_scheduler.h"
#include "stdlib.h"
#include "stdbool.h"
#include "kernel/rosa_tim.h"

//tcb *TaskHandleID[21];//Array for the HandleIDs used by Task Create function when returning ID
// Index represents the ID!!!
tcb *EXECTASK=NULL;//Currently executing task
tcb *READY=NULL;
tcb *BLOCKED=NULL;
tcb *WAITING=NULL;
tcb *SUSPENDED=NULL;
//tcb Idle_tcb;

#define IDLE_STACK_SIZE 0x40
#define IDLE_PRIORITY 0
#define HIGHEST_PRIORITY 21

void scheduler(void);
bool ChangeIdleTaskPriorityToHighest(void);
bool ChangeIdleTaskPriorityToLowest(void);
void Resume_Suspended(int handleID);
void Insert_Supsended(int handleID);
void Check_Waiting_Queue();
void Chose_A_Task_Frome_Ready_Queue();
void Insert_Waiting(tcb *TASK);
void Insert_Ready(tcb *TASK);
void Check_Blocked_Queue();
void Insert_Blocked();
void Idle_Task(void);
bool ROSA_SchedulerResume(void);
bool ROSA_SchedulerSuspend(void);
bool ROSA_Scheduler(void);

void Idle_Task(void){
	while(1){
	}
}
bool ROSA_Scheduler(void){
	//Uncomment line below when Vildan gives implementation for the function
	//Create Idle task with already existing tcb
	//Idle_tcb.handleID = ROSA_TaskCreate("Idle", &Idle_Task, IDLE_STACK_SIZE,IDLE_PRIORITY);//Idle TASK
	interruptDisable();//ROSA function
	scheduler();
	interruptEnable();//ROSA function
	return true;
}
bool ROSA_SchedulerSuspend(void){
//Problem it needs to run only certain amount of time!!! if not it will block the processor indefinitely  
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
void scheduler(void){
//When calling Delay it puts a task in waiting queue same for other functions
//Suspend task, resume task, semaphore block etc...
	
	interruptDisable();
	
	//All functions take appropriate task from queue and move it in the Ready queue in a sorted manner
	Check_Blocked_Queue(); 
	Check_Waiting_Queue();
	
	Chose_A_Task_Frome_Ready_Queue();//If a task is the same prio it goes at the end of the tasks with same prio's
	//This is done for Round Robin scheduling
	
	interruptEnable();
}


void Check_Blocked_Queue(){
	
}
void Insert_Blocked(){	
//NOTE before calling this function, waitSemaphore in the EXECTASK should have already been updated
	tcb *Previous=BLOCKED, *Temp=BLOCKED;
	if(Temp==NULL){//If the list is empty put it in first 
		Temp=EXECTASK;
		return;
	}
	if(EXECTASK->waitSemaphore<Temp->waitSemaphore){//If the task should be put in the beginning of the list
		BLOCKED=EXECTASK;
		EXECTASK->nexttcb=Temp;
		return;
	}
	while(Temp!=NULL){
		if (Temp->waitSemaphore>=EXECTASK->waitSemaphore){
			EXECTASK->nexttcb=Temp;
			Previous->nexttcb=EXECTASK;
			return;
		}
		Previous=Temp;
		Temp=Temp->nexttcb;
	}
	//If the task should be put at the end of the list
	EXECTASK->nexttcb=Temp;
	Previous->nexttcb=EXECTASK;
	return;	
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
	if(Temp==NULL){//If the list is empty put it in first 
		WAITING=TASK;
		return;
	}
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
	//If the task should be put at the end of the list
	Temp->nexttcb=TASK;
	TASK->nexttcb=NULL;
	return;
}
void Insert_Ready(tcb *TASK){
	int prio=TASK->priority;
	tcb *Temp=READY;
	if(Temp==NULL){//If the list is empty put it in first 
		READY=TASK;
		return;
	}
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
	//Idle task will always be at the end of the list
}
