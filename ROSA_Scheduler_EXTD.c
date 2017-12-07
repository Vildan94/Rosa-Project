#include "ROSA_Scheduler_EXTD.h"
#include "rosa_int.h"
#include "rosa_ker.h"

bool ChangeIdleTaskPriorityToHighest(void);
bool ChangeIdleTaskPriorityToLowest(void);
void Create_Queues(void); 
void Scheduler(void);

#define IDLE_STACK_SIZE 0x40
#define IDLE_PRIORITY 0
#define HIGHEST_PRIORITY 21
static int Idle_stack[IDLE_STACK_SIZE];
static tcb Idle_tcb; //Change the type when Vildan creates struct!!

struct Linked_List_Ready{
	struct tcb *Task_tcb;
	struct Linked_List_Ready *Next_element;
}
struct Linked_List_Suspend{
	struct tcb *Task_tcb;
	struct Linked_List_Suspend *Next_element;
}
struct Linked_List_Blocked{
	struct tcb *Task_tcb;
	TimerTick Semaphore_Waiting_Time;
	struct Linked_List_Blocked *Next_element;
}
struct Linked_List_Waiting{
	struct tcb *Task_tcb;
	TimerTick Task_waiting_Time;
	struct Linked_List_Waiting *Next_element;
}


void Idle_Task(void){
	while(1){	
	}
}

bool ROSA_Scheduler(void){
	Idle_tcb.staddr()=&Idle_Task();
	Idle_tcb.handleID = ROSA_TaskCreate("Idle", &Idle_Task, IDLE_STACK_SIZE,IDLE_PRIORITY);//Idle TASK 
	interruptDisable(void);//ROSA function
	Create_Queues(); //Create Ready,Suspend,Blocked and Waiting Queues
	interruptEnable(void);//ROSA function
	//Scheduler();
	return true;
}
bool ROSA_SchedulerSuspend(void){
	ROSA_yield();
	if(ChangeIdleTaskPriorityToHighest())
	return true;
	else
	return false;
}
bool ROSA_SchedulerResume(void){
	ROSA_yield();
	if(ChangeIdleTaskPriorityToLowest())
	return true;
	else
	return false;
}
bool ChangeIdleTaskPriorityToHighest(void){
	Idle_tcb.priority=HIGHEST_PRIORITY;
	return true;
}
bool ChangeIdleTaskPriorityToLowest(void){
	Idle_tcb.priority=IDLE_PRIORITY;
	return true;
}
void Create_Queues(void){
	Linked_List_Ready *Ready_Head_Node;
	Ready_Head_Node=NULL; //Empty list
	Linked_List_Blocked *Blocked_Head_Node;
	Blocked_Head_Node=NULL; //Empty list
	Linked_List_Suspend *Suspend_Head_Node;
	Suspend_Head_Node=NULL; //Empty list
	Linked_List_Waiting *Waiting_Head_Node;
	Waiting_Head_Node=NULL; //Empty list
}
void Scheduler(void){
	
}