#include "kernel/rosa_scheduler.h"
#include "kernel/rosa_ker.h"
#include "stdbool.h"

bool ChangeIdleTaskPriorityToHighest(void);
bool ChangeIdleTaskPriorityToLowest(void);

void scheduler(void){	
	Check_Waiting_Queue();
	Choose_A_Task_From_Ready_Queue();//If a task is the same prio it goes at the end of the tasks with same prio's
}
bool ROSA_Scheduler(void){
	TCBLIST=READY;
	READY=READY->nexttcb;//Changes the TCBLIST to Ready so the task gets removed from the top of the Ready queue
	ROSA_start();
	return true;
}
//bool ROSA_SchedulerSuspend(void){
	////Problem it needs to run only certain amount of time!!! if not it will block the processor indefinitely
	//if(ChangeIdleTaskPriorityToHighest()){
		//ROSA_yield();
		//return true;
	//}
	//else
	//return false;
//}
//bool ROSA_SchedulerResume(void){
	//if(ChangeIdleTaskPriorityToLowest()){
		//Insert_Ready(EXECTASK);
		//ROSA_yield();
		//return true;
	//}
	//else
	//return false;
//}
bool ChangeIdleTaskPriorityToHighest(void){//WONT WORK HOW DO YOU GET OUT FROM TASK?!
	TaskHandleID[0]->priority=HIGHEST_PRIORITY;
	return true;
}
bool ChangeIdleTaskPriorityToLowest(void){
	TaskHandleID[0]->priority=IDLE_PRIORITY;
	return true;
}
void Check_Waiting_Queue(){
	tcb *Temp=WAITING,*Help;
	if(WAITING==NULL)
	return;
	if(Temp->waitUntil>SystemTime) //If tasks waiting time is bigger than systems time then no task will migrate to ready queue
	return;
	while(Temp!=NULL && Temp->waitUntil<=SystemTime){//If task EXSISTS and its waiting time is lower or equal then
		//Help=Temp;
		WAITING=WAITING->nexttcb;//Unchain it from the list (since it is always the first element) and assign ptr of the next task to the WAITING
		Insert_Ready(Temp);//Move it to the ready queue
		Temp=WAITING;//Change the temp node to the beginning of the queue
	}
}
void Choose_A_Task_From_Ready_Queue(){
	if(READY !=NULL){
		EXECTASK=READY ;//Executing ptr to the first node in ready queue
		READY =READY ->nexttcb;//Queue should now point to the second node (UNLINK THE FIRST ELEMENT)
		EXECTASK->nexttcb=NULL;//Pointer in the tcb structure shouldn't point nowhere =NULL
	}
}
void Insert_Ready(tcb *TASK){
	tcb *TEMP=READY,*PREVIOUS=READY;
	if(READY==NULL){
		READY=TASK;
		TASK->nexttcb=NULL;
		return;
	}
	if(TASK->priority==IDLE_PRIORITY){//Inserting the IDLE Task
		while(TEMP!=NULL){
			PREVIOUS=TEMP;
			TEMP=TEMP->nexttcb;
		}
		TASK->nexttcb=NULL;
		PREVIOUS->nexttcb=TASK;		
		return;
	}
	if(TASK->priority>READY->priority){
		TASK->nexttcb=READY;
		READY=TASK;
		return;		
	}
	while(TEMP!=NULL){
		if(TASK->priority>TEMP->priority){
			TASK->nexttcb=TEMP;
			PREVIOUS->nexttcb=TASK;
			return;
		}
		PREVIOUS=TEMP;
		TEMP=TEMP->nexttcb;
	}
}
void Insert_Waiting(tcb *TASK){
	int waitUntil=TASK->waitUntil;
	tcb *Temp=WAITING;
	if(Temp==NULL){//If the list is empty put it in first
		WAITING=TASK;
		WAITING->nexttcb=NULL;
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
	while(Temp!=NULL){//The rest of the elements
		if(Temp->handleID==handleID){
			Previous->nexttcb=Temp->nexttcb;
			Temp->nexttcb=SUSPENDED;
			SUSPENDED=Temp;
			return;
		}
		Previous=Temp;
		Temp=Temp->nexttcb;
	}
	
	////BLOCKED SEARCH
	//Temp=BLOCKED;
	//Previous=BLOCKED;
	//if(Temp!=NULL && Temp->handleID==handleID){//If it is the first element
		//BLOCKED=Temp->nexttcb;
		//Temp->nexttcb=SUSPENDED;
		//SUSPENDED=Temp;
	//}
	//while(Temp->nexttcb!=NULL){//The rest of the elements
		//if(Temp->nexttcb->handleID==handleID){
			//Temp=Temp->nexttcb;
			//Previous->nexttcb=Temp->nexttcb;
			//Temp->nexttcb=SUSPENDED;
			//SUSPENDED=Temp;
			//return;
		//}
		//Temp=Temp->nexttcb;
	//}
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