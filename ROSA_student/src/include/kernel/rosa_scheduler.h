#ifndef _scheduler_H_
#define _scheduler_H_

#include "kernel/rosa_ker.h"
#include "stdbool.h"
#include "rosa_int.h"
#include "rosa_ker.h"

extern tcb *TaskHandleID[21];//Array for the HandleIDs used by Task Create function when returning ID
// Index represents the ID!!!
extern tcb *EXECTASK;//Currently executing task
extern tcb *READY;
extern tcb *BLOCKED;
extern tcb *WAITING;
extern tcb *SUSPENDED;
extern tcb Idle_tcb;

void scheduler(void);//This function shouldn't be available to the user!!!

bool ROSA_Scheduler(void);
bool ROSA_SchedulerSuspend(void);
bool ROSA_SchedulerResume(void);

#endif 
