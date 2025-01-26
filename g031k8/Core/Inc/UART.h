#ifndef INC_UART_H_
#define INC_UART_H_

//INCLUDES
#include <stdint.h>
#include "config.h"
#include "utility.h"

//VARIABLE DECLARATIONS
volatile extern char rx_buffer[1];
volatile extern enum Validate UART_DMA_TX_is_complete;

//FUNCTION DECLARATIONS
uint8_t Start_UART_Receive(void);

#endif /* INC_UART_H_ */
