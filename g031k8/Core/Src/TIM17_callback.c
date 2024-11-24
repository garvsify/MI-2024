//INCLUDES
#include "TIM17_callback.h"
#include "system.h"

//VARIABLES

//FUNCTIONS
void TIM17_callback(TIM_HandleTypeDef *htim)
{
	//Start ADC (in scan mode) conversion
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCResultsDMA, (uint32_t)num_ADC_conversions);

	//TMR1_Stop(); //-to be sorted

	//size_t tmr3_value = TMR3_OVERFLOW_COUNT; //-to be sorted

	//TMR3_Write(tmr3_value); //-to be sorted
	//TMR3_Start(); //time delay to allow ADCC conversion to complete, DMA is triggered on overflow. //-to be sorted
}
