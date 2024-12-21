#ifndef INC_CUSTOM_CALLBACKS_H_
#define INC_CUSTOM_CALLBACKS_H_

#include "system.h"

void TIM16_callback(TIM_HandleTypeDef *htim);
void ADC_DMA_conversion_complete_callback(ADC_HandleTypeDef *hadc);
uint8_t Multiply_Duty_By_Current_Depth_and_Divide_By_256(void);

#endif /* INC_CUSTOM_CALLBACKS_H_ */
