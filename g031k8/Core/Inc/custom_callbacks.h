#ifndef INC_CUSTOM_CALLBACKS_H_
#define INC_CUSTOM_CALLBACKS_H_

#include "system.h"

void TIM16_callback(TIM_HandleTypeDef *htim);
void TIM17_callback(TIM_HandleTypeDef *htim);
void ADC_DMA_conversion_complete_callback(ADC_HandleTypeDef *hadc);

#endif /* INC_CUSTOM_CALLBACKS_H_ */
