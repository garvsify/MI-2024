#ifndef INC_TIMERS_H_
#define INC_TIMERS_H_

//INCLUDES
#include <stdint.h>

#include "config.h"

//FUNCTION DECLARATIONS
uint8_t Start_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel);
uint8_t Stop_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel);

#endif /* INC_TIMERS_H_ */
