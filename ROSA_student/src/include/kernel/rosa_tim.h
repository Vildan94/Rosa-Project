#ifndef _ROSA_TIMER_H_
#define _ROSA_TIMER_H_

#include <avr32/io.h>
#include "stdbool.h"

//typedef unsigned long long TimerTick;
#define TimerTick unsigned long long
#define TIMERTICK_MAXVAL 0xFFFFFFFFFFFFFFFE

TimerTick SystemTime;

TimerTick ROSA_getTimerTicks(void);
bool ROSA_DelayUntil(TimerTick * lastWakeTime, TimerTick period);
bool ROSA_Delay(TimerTick ticks);



extern void timerInit(unsigned int);
extern void timerReset(void);
extern void timerStart(void);
extern void timerStop(void);

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

#endif 
