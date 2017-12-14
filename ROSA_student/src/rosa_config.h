
#ifndef _ROSA_CONFIG_H_
#define _ROSA_CONFIG_H_

#include <avr32/io.h>
#include "drivers/debug.h"

//Maximum number of chars in the task id (the task name)
#define NAMESIZE 4
#define TIMER_RESOLUTION 1

//Which debug level will output from the various dgbXYZ() functions be?
//Valid values are DEBUGLEVEL0-DEBUGLEVEL3
#define DEBUGLEVEL DEBUGLEVEL1

//USART configures
#define USART				(&AVR32_USART0)
#define USART_BAUDRATE 		57600
#define USART_CHARLEN		8
#define USART_RX_PIN		AVR32_USART0_RXD_0_0_PIN
#define USART_RX_FUNCTION	AVR32_USART0_RXD_0_0_FUNCTION
#define USART_TX_PIN		AVR32_USART0_TXD_0_0_PIN
#define USART_TX_FUNCTION	AVR32_USART0_TXD_0_0_FUNCTION

#define FOSC0 12000000		//CPU clock frequencey

#endif /* _ROSA_CONFIG_H_ */
