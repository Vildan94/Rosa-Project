#ifndef _ROSA_INT_H_
#define _ROSA_INT_H_

#include <avr32/io.h>
//Interrupt initialization must be done before enabling interrupts
extern void interruptInit(void);
//Disable interrupts
extern void interruptDisable(void);
//Enable interrupts
extern void interruptEnable(void);
//Check if interrupts are enabled.
extern int  isInterruptEnabled(void);
void ROSA_yieldFromISR(void);

#endif 
