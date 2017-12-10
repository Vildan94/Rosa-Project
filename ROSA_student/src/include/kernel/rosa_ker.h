#ifndef _ROSA_KER_H_
#define _ROSA_KER_H_

#include "rosa_def.h"
#include "rosa_scheduler.h"
#include "stdlib.h"
#include "stdbool.h"

extern tcb * TCBLIST;
extern tcb * EXECTASK;

extern void ROSA_contextInit(tcb * tcbTask);
extern void ROSA_yield(void);

#define ROSA_INITIALSR 0x1c0000

__attribute__((__interrupt__)) extern void timerISR(void);
void ROSA_init(void);
void ROSA_tcbCreate(tcb * tcbTask, char tcbName[NAMESIZE], void *tcbFunction, int * tcbStack, int tcbStackSize);
extern void ROSA_tcbInstall(tcb *task);
extern void ROSA_start(void);

extern int ROSA_TaskCreate(char ID[NAMESIZE], void *functionPtr, int stackSize, int priority);
extern bool ROSA_TaskDelete(int HandleId );
extern bool ROSA_TaskSusspend(int HandleId);
extern bool ROSA_TaskResume(int HandleId);

#endif 
