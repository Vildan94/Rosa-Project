#ifndef _ROSA_TIMER_H_
#define _ROSA_TIMER_H_

#include <avr32/io.h>
#include "stdbool.h"

typedef unsigned long long TimerTick;
TimerTick SystemTime;
#define TIMERTICK_MAXVAL 0xFFFFFFFFFFFFFFE

/***********************************************************
 * Kernel timer functions
 ***********************************************************/
extern void timerInit(unsigned int);
extern void timerReset(void);
extern void timerStart(void);
extern void timerStop(void);
bool ROSA_Delay(TimerTick ticks);
bool ROSA_DelayUntil(TimerTick * lastWakeTime, TimerTick period);
TimerTick ROSA_getTickTime(void);



//The timer interrupt service routine
void timerISR(void);
extern void timerClearInterrupt(void);


//Timer period functions
int timerPeriodSet(unsigned int ms);
extern void timerPrescaleSet(int);
extern void timerRCSet(int);

//Timer period variables
extern int timerPrescale;
extern int timerRC;

#endif /* _ROSA_TIMER_H_ */
