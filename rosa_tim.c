/*****************************************************************************

                 ,//////,   ,////    ,///' /////,
                ///' ./// ///'///  ///,    ,, //
               ///////,  ///,///   '/// //;''//,
             ,///' '///,'/////',/////'  /////'/;,

    Copyright 2010 Marcus Jansson <mjansson256@yahoo.se>

    This file is part of ROSA - Realtime Operating System for AVR32.

    ROSA is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ROSA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ROSA.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
/* Tab size: 4 */

#include "rosa_config.h"
#include "drivers/delay.h"
#include "kernel/rosa_int.h"
#include <limits.h>

//typedef unsigned long long TimerTick;
#define TimerTick unsigned long long
#define TIMERTICK_MAXVAL ULLONG_MAX

TimerTick SystemTime = 0;


/***********************************************************
 * timerInterruptHandler
 *
 * Comment:
 * 	This is the basic timer interrupt service routine.
 **********************************************************/
__attribute__((__interrupt__))
void timerISR(void)
{
	int sr;
	volatile avr32_tc_t * tc = &AVR32_TC;
	
	
	SystemTime++;	//increments the SystemTime
	
	//Read the timer status register to determine if this is a valid interrupt
	sr = tc->channel[0].sr;
	if(sr & AVR32_TC_CPCS_MASK)
		ROSA_yieldFromISR();	//This saves context, calls the scheduler, switches over to next
}


/***********************************************************
 * timerPeriodSet
 *
 * Comment:
 * 	Set the timer period to 'ms' milliseconds.
 *
 **********************************************************/
int timerPeriodSet(unsigned int ms)
{
	int rc, prescale;
	int f[] = { 2, 8, 32, 128 };
	//FOSC0 / factor_prescale * time[s];
	prescale = AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK5;
	rc = FOSC0 / f[prescale - 1] * ms / 1000;
	timerPrescaleSet(prescale);
	timerRCSet(rc);
	return rc * prescale / FOSC0;
}



int ROSA_Delay(TimerTick ticks)
{
	TimerTick newWakeTime;
	TimerTick timeUntilOverflow;
	
	//CRITICAL SECTION
	timeUntilOverflow = TIMERTICK_MAXVAL - SystemTime;
	
	if(timeUntilOverflow < ticks){
		newWakeTime = ticks - timeUntilOverflow;
	}
	else{
		newWakeTime = SystemTime + ticks;	//add delay-length value to current time
	}
	//END OF CRITICAL SECTION
	
	//Save newWakeTime to the task's WakingTime attribute (executing task's ID is saved in a global variable)
	//Put the task into the Waiting Queue
<<<<<<< HEAD
	//Call the scheduler
=======
	//Call the scheduler (ex: yield)
>>>>>>> 8be64dd6ec8c7bb1abe4b2c2fedbec81211d5307
}

int ROSA_DelayUntil(TimerTick * lastWakeTime, TimerTick period)
{
	TimerTick newWakeTime;
	TimerTick timeUntilOverflow;
	
	//CRITICAL SECTION
	timeUntilOverflow = TIMERTICK_MAXVAL - *lastWakeTime;
	
	if(timeUntilOverflow < period){
		newWakeTime = period - timeUntilOverflow;
	}
	else{
		newWakeTime = *lastWakeTime + period;	//add delay-length value to current time
	}
	//END OF CRITICAL SECTION
	
	//Save delayEnd to the task's WakingTime attribute (executing task's ID is saved in a global variable)
	//Put the task into the Waiting Queue
	//Call the scheduler	
<<<<<<< HEAD
	*lastWakeTime = SystemTime	//Calculate new lastWakeTime value
=======
	*lastWakeTime = SystemTime;	//Calculate new lastWakeTime value
>>>>>>> 8be64dd6ec8c7bb1abe4b2c2fedbec81211d5307
}

TimerTick ROSA_getTimerTicks(void)
{
	//Read the nr of ticks
	return SystemTime;
}