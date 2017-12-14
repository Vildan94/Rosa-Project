#include <avr32/io.h>
#include "kernel/rosa_ker.h"
#include "kernel/rosa_tim.h"
#include "drivers/led.h"
#include "drivers/delay.h"
#include "drivers/usart.h"
#include "rosa_config.h"
#include "kernel/rosa_scheduler.h"

#define T_STACK_SIZE 0x40
int Task1_Handle,Task2_Handle,Task3_Handle,Task4_Handle=0;

void task4(void){
	while(1) {
		ledToggle(LED3_GPIO);
		ROSA_Delay(2000);
	}
}
void task1(void){
	TimerTick LastWakeTime=ROSA_getTickTime();
	while(1) {		
		ledToggle(LED0_GPIO);
		ROSA_DelayUntil(&LastWakeTime,250);
		ledToggle(LED0_GPIO);
		ROSA_DelayUntil(&LastWakeTime,2500);		
	}
}
void task2(void){
	while(1) {
		ledToggle(LED1_GPIO);
		ROSA_Delay(50);
		ROSA_TaskDelete(Task2_Handle);
	}
}
void task3(void){
	while(1) {
		ledToggle(LED2_GPIO);
		
		if(Task4_Handle==0)
		Task4_Handle=ROSA_TaskCreate("tsk4",task4,T_STACK_SIZE,6);
		Task2_Handle=ROSA_TaskCreate("tsk2",task2,T_STACK_SIZE,6);
		
		ROSA_Delay(5000);
	}
}
int main(void){
	ROSA_init();
	Task1_Handle=ROSA_TaskCreate("tsk1",task1,T_STACK_SIZE,6);
	Task2_Handle=ROSA_TaskCreate("tsk2",task2,T_STACK_SIZE,6);
	Task3_Handle=ROSA_TaskCreate("tsk3",task3,T_STACK_SIZE,6);
	ROSA_Scheduler();
	while(1);
}
