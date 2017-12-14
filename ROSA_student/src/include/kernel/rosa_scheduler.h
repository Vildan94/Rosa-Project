#ifndef _scheduler_H_
#define _scheduler_H_

#include "kernel/rosa_ker.h"
#include "stdbool.h"

#define IDLE_PRIORITY 0
#define HIGHEST_PRIORITY 21

bool ROSA_Scheduler(void);
bool ROSA_SchedulerSuspend(void);
bool ROSA_SchedulerResume(void);
void scheduler(void);

#endif 
