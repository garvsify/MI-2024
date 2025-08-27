/*NOTES ON THIS BUILD:


Issues:


Potential Issues:


*/


//INCLUDES
#include "main.h"

int __attribute__((optimize("O0")))main(void)
{
	//SYSTEM INIT
	System_Init();

	//STARTUP
	Startup();

	while (1)
	{
		Check_Software_Timers();

		if(Get_Status_Bit(&statuses, DMA_To_Start) == YES){

			Clear_Status_Bit(&statuses, DMA_To_Start);
			HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);
		}
	}

	return 1;
}



