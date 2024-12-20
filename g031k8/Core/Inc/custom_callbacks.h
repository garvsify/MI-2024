#ifndef INC_CUSTOM_CALLBACKS_H_
#define INC_CUSTOM_CALLBACKS_H_

#include "system.h"

void TIM16_callback(TIM_HandleTypeDef *htim);
uint8_t Multiply_Duty_By_Current_Depth_and_Divide_By_256(void);

#endif /* INC_CUSTOM_CALLBACKS_H_ */
