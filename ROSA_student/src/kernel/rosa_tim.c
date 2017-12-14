#include "rosa_config.h"
#include "drivers/delay.h"
#include "kernel/rosa_int.h"
#include "drivers/led.h" //used for testing the TIMER ISR
#include "kernel/rosa_ker.h"
#include "stdbool.h"
#include "kernel/rosa_tim.h"

TimerTick SystemTime=0;

__attribute__((__interrupt__)) void timerISR(void)
{
	int sr;
	volatile avr32_tc_t * tc = &AVR32_TC;
	//increments the SystemTime, resets before overflow
	if(SystemTime+1 != TIMERTICK_MAXVAL)
	SystemTime++;
	else
	SystemTime = 0;
	ledToggle(LED4_GPIO);
	//Read the timer status register to determine if this is a valid interrupt
	sr = tc->channel[0].sr;
	if(sr & AVR32_TC_CPCS_MASK){
		Insert_Ready(EXECTASK);
		ROSA_yieldFromISR();
	}

}

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

bool ROSA_Delay(TimerTick ticks){
	TimerTick newWakeTime=0;
	TimerTick timeUntilOverflow=0;	
	//CRITICAL SECTION
	timeUntilOverflow = TIMERTICK_MAXVAL - SystemTime;	
	if(ticks > timeUntilOverflow){
		newWakeTime = ticks - timeUntilOverflow;
	}
	else{
		newWakeTime = SystemTime + ticks;	//add delay-length to current time
	}
	//END OF CRITICAL SECTION	
	EXECTASK->waitUntil = newWakeTime;	//Save wake time into task's attribute	
	Insert_Waiting(EXECTASK);	//Put the task into the Waiting Queue	
	ROSA_yield();		//Call the scheduler (ex: yield)	
	return true;
}

bool ROSA_DelayUntil(TimerTick * lastWakeTime, TimerTick period)
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
	
	EXECTASK->waitUntil = newWakeTime;	//Save waking time in the task's attribute
	
	Insert_Waiting(EXECTASK);				//Put the task into the Waiting Queue
	
	ROSA_yield();					//Call the scheduler
	
	*lastWakeTime = SystemTime;		//When task gets to execute again, calculate new lastWakeTime value before exciting
	
	return true;
}


TimerTick ROSA_getTickTime(void){
	return SystemTime;	
}