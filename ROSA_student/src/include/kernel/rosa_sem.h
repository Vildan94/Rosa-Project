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
handleID ROSA SemaphoreBinaryCreate();
bool ROSA SemaphoreBinaryTake(handleID ID, timerTick ticksToWait);
bool ROSA SemaphoreBinaryRelease (handleID ID);
bool ROSA SemaphoreDelete (handleID ID);
void ROSA SemaphoreCeil (handleID ID, tcb *tcbtask);
handleID ROSA SemaphoreIPCPCreate ();
bool ROSA SemaphoreIPCPTake (handleID ID, timerTick ticksToWait);
bool ROSA SemaphoreIPCPRelease (handleID ID);