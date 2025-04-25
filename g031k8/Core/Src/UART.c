//INCLUDES
#include "UART.h"

//VARIABLE DEFINITIONS
volatile uint8_t rx_buffer[1] = {0};
volatile enum Validate UART_DMA_TX_is_complete = YES;

//FUNCTION DEFINITIONS
uint8_t Start_UART_Receive(void){

	//START UART RECEIVE
	HAL_UART_Receive_DMA(&huart2, (uint8_t*)rx_buffer, sizeof(rx_buffer));

	return 1;
}

