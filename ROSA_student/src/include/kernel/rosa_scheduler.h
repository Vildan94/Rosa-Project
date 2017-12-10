#ifndef _scheduler_H_
#define _scheduler_H_

#include "kernel/rosa_ker.h"
#include "stdbool.h"
#include "rosa_int.h"
#include "rosa_ker.h"

void scheduler(void);//This function shouldn't be available to the user!!!

bool ROSA_Scheduler(void);
bool ROSA_SchedulerSuspend(void);
bool ROSA_SchedulerResume(void);

#endif 
