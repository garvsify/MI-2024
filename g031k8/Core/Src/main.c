/*NOTES ON THIS BUILD:

***This won't build, just committing to show the code test I've done (using code at the bottom of main.c)***

*/

//INCLUDES
#include "system.h"

volatile char rx_buffer[1] = {0};
const char one_byte_data = 'j';

int main(void)
{
	//SYSTEM INIT
	System_Init();

	//STARTUP BITS AND BOBS
	Startup();

	//START FREQ. GEN and PWM GEN TIMERS and ENABLE PWM OUTPUT
	Start_PWM_Gen_Timer_Main_and_Secondary_Oscillators(&htim1, TIM_CHANNEL_2, TIM_CHANNEL_4);
	Start_Freq_Gen_Timer();
	Start_Input_Capture_Timer();

	//ENABLE INTERRUPTS
	Global_Interrupt_Enable();

	//START TAP TEMPO SWITCH MONITORING AND UART RECEIVE
	Start_Tap_Tempo_Monitoring_Timers_and_UART_Receive();

	while (1)
	{
		/*if(UART_DMA_TX_is_complete == YES){

			UART_DMA_TX_is_complete = NO;
			HAL_UART_Transmit_DMA(&huart2, (const uint8_t*)&one_byte_data, sizeof(one_byte_data));
		}*/
	}
	return 1;
}

#include <stdio.h>
#include <stdint.h>

uint8_t isPrime(uint16_t x);

uint16_t periods[129] = {0};
uint8_t check[129] = {0};

int main()
{
    //initialise period values in array
	for(uint8_t p = 0; p < 129; p++){

		periods[p] = 256 - p;
	}

    for(uint16_t u = 0; u < 66048-8256; u++){
        if(isPrime(66048-u) == 0){
            for(uint8_t i = 0; i < 129; i++){
                if((66048 - u) % periods[i] == 0){
                	check[i] = 1;
	            }
	        }
	    }
	}

	for(uint8_t p = 0; p < 129; p++){

		printf("%d\n",check[p]);
	}

    return 0;
}


uint8_t isPrime(uint16_t x){
    for (int d = 2; d * d <= x; d++) {
        if (x % d == 0)
            return 0;
    }
    return x >= 2;
}



