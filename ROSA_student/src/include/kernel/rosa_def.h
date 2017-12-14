#ifndef rosa_def_H_
#define rosa_def_H_

#include "rosa_config.h"
#include "kernel/rosa_tim.h"

#ifndef NULL
#define NULL 0
#endif

typedef struct tcb_record_t {
	struct tcb_record_t * nexttcb;
	char id[NAMESIZE];		//The task id/name
	void (*staddr) (void);	//Start address
	int *dataarea;			//The stack data area
	int datasize;			//The stack size
	int *saveusp;			//The current stack position
	int SAVER0;				//Temporary work register
	int SAVER1;				//Temporary work register
	int savesr;				//The current status register
	int retaddr;			//The return address
	int savereg[15];		//The CPU registers
	
	TimerTick waitUntil;
	TimerTick waitSemaphore;
	int priority;
	int handleID;	
} tcb;

#endif 
