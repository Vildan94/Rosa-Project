#ifndef _ROSA_SEM_H_
#define _ROSA_SEM_H_

#include <avr32/io.h>

//Kernel includes
#include "kernel/rosa_ker.h"
#include "kernel/rosa_int.h"
//I/O driver includes
#include "drivers/led.h"
#include "drivers/delay.h"
//Include configuration
#include "rosa_config.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "rosa_tim.h"
#include "rosa_scheduler.h"
#include "rosa_ker.h"

#define Taken 1
#define Free 0

struct semaphore {
	int ceil;
	int ID;
	int state;
	int taskPriority;
	int isBinary;
};

typedef struct semaphore Semaphore;
typedef Semaphore * semaphoreHandler;

typedef unsigned long long timerTick;
typedef int handleID;

// FUNCTIONS
handleID * createArray();
handleID ROSA_SemaphoreBinaryCreate();
bool ROSA_SemaphoreBinaryTake(handleID ID, timerTick ticksToWait);
bool ROSA_SemaphoreBinaryRelease (handleID ID);
bool ROSA_SemaphoreDelete (handleID ID);
void ROSA_SemaphoreCeil (handleID ID, tcb *tcbtask);
handleID ROSA_SemaphoreIPCPCreate ();
bool ROSA_SemaphoreIPCPTake (handleID ID, timerTick ticksToWait);
bool ROSA_SemaphoreIPCPRelease (handleID ID);

#endif /* _ROSA_SEM_H_ */