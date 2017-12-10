#include "kernel/rosa_int.h"
#include "kernel/rosa_scheduler.h"

void contextSaveFromISR(void);
void contextRestoreFromISR(void);

void ROSA_yieldFromISR(void)
{
	contextSaveFromISR();	  //Save the task context
	scheduler();			  //Find next task to execute
	contextRestoreFromISR();  //...and switch over to it.
}
