/*
 * rosa_testc_sem.c
 *
 * Created: 2017-12-12 16:05:43
 *  Author: mgo12003
 */ 

#include "kernel/rosa_sem.h"

// CREATING SEMAPHORES + NESTING + DELETION

handleID sem1, sem2, sem3, semIPCP1, semIPCP2, semIPCP3;
TimerTick ten, nowaiting, forever;
bool val;

ten = 10;
nowaiting = 0;
forever =  TIMERTICK_MAXVAL;

sem1 = -1;
sem2 = -1;
sem3 = -1;

sem1 = ROSA_SemaphoreBinaryCreate();
sem2 = ROSA_SemaphoreBinaryCreate();
sem3 = ROSA_SemaphoreBinaryCreate();

if (sem1 != -1)
{
	// LED0 TURN ON
}
if (sem2 != -1)
{
	// LED1 TURN ON
}
if (sem3 != -1)
{
	// LED2 TURN ON
}

// LED1 TURN OFF
// LED2 TURN OFF
// LED3 TURN OFF

val = ROSA_SemaphoreBinaryTake(sem1, ten);
if (val == true)
{
	// LED0 TURN ON
}
val = ROSA_SemaphoreBinaryTake(sem2, ten);
if (val == true)
{
	// LED1 TURN ON
}

val = ROSA_SemaphoreBinaryRelease (sem2);
if (val == true)
{
	// LED1 TURN OFF
}

val = ROSA_SemaphoreBinaryRelease (sem1);
if (val == true)
{
	// LED0 TURN OFF
}

val = ROSA_SemaphoreBinaryTake(sem3, ten);
if (val == true)
{
	// LED2 TURN ON
}
val = ROSA_SemaphoreBinaryRelease (sem3);
if (val == true)
{
	// LED2 TURN OFF
}

val = ROSA_SemaphoreDelete (sem2);
if (val == true)
{
	if (sem2 == NULL)
	{
		// LED 3 TURN ON
	}
}
// LED 3 TURN OFF


//////////////////////////////////////////////////////////////////////////

semIPCP1 = -1;
semIPCP2 = -1;
semIPCP3 = -1;

semIPCP1 = ROSA_SemaphoreIPCPCreate ();
semIPCP2 = ROSA_SemaphoreIPCPCreate ();
semIPCP3 = ROSA_SemaphoreIPCPCreate ();

if (semIPCP1 != -1)
{
	// LED0 TURN ON
}
if (semIPCP2 != -1)
{
	// LED1 TURN ON
}
if (semIPCP3 != -1)
{
	// LED2 TURN ON
}

// LED1 TURN OFF
// LED2 TURN OFF
// LED3 TURN OFF

/////////

val = ROSA_SemaphoreIPCPTake(semIPCP1, nowaiting);
if (val == true)
{
	// LED0 TURN ON
}
val = ROSA_SemaphoreIPCPTake(semIPCP2, ten);
if (val == true)
{
	// LED1 TURN ON
}

val = ROSA_SemaphoreIPCPTake(semIPCP2, forever);
if (val == false)
{
	// CORRECT AND SHOULD BLOCK
	// LED1 BLINKING
}

val = ROSA_SemaphoreIPCPRelease(semIPCP2);
if (val == true)
{
	// LED1 TURN OFF
}

// SHOULD SEMIPCP2 be Taken now by the task wanting to wait forever? Or how does it work?!


val = ROSA_SemaphoreIPCPRelease(semIPCP1);
if (val == true)
{
	// LED0 TURN OFF
}

val = ROSA_SemaphoreIPCPTake(semIPCP3, forever);
if (val == true)
{
	// LED2 TURN ON
}
val = ROSA_SemaphoreBinaryRelease (semIPCP3);
if (val == true)
{
	// LED2 TURN OFF
}

val = ROSA_SemaphoreDelete (semIPCP1);
if (val == true)
{
	if (semIPCP1 == NULL)
	{
		// LED 3 TURN ON
	}
}
// LED 3 TURN OFF
