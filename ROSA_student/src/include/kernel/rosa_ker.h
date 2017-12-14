#ifndef _ROSA_KER_H_
#define _ROSA_KER_H_

#include "rosa_def.h"
#include "stdbool.h"

#define ROSA_INITIALSR 0x1c0000

//Array for the HandleIDs used by Task Create function when returning ID
// Index represents the ID!!!
extern tcb *TaskHandleID[21];
extern tcb * TCBLIST;
extern tcb * READY;
extern tcb * EXECTASK;
extern tcb *BLOCKED;
extern tcb *WAITING;
extern tcb *SUSPENDED;

extern void ROSA_contextInit(tcb * tcbTask);
extern void ROSA_yield(void);

int ROSA_TaskCreate (char ID[NAMESIZE], void *functionPtr, int stackSize, int priority);
bool ROSA_TaskSusspend(int HandleId);
bool ROSA_TaskResume(int HandleId);
bool ROSA_TaskDelete (int HandleId);
void Idle_Task(void);

__attribute__((__interrupt__)) extern void timerISR(void);
void ROSA_init(void);
void ROSA_tcbCreate(tcb * tcbTask, char tcbName[NAMESIZE], void *tcbFunction, int * tcbStack, int tcbStackSize,int priority);
extern void ROSA_tcbInstall(tcb *task);
extern void ROSA_start(void);

#endif 
