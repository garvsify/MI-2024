#ifndef INC_TIMERS_H_
#define INC_TIMERS_H_

//INCLUDES
#include <stdint.h>

#include "config.h"
#include "utility.h"

//FUNCTION DECLARATIONS
uint8_t __attribute__((optimize("O0")))Start_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel);
uint8_t __attribute__((optimize("O0")))Stop_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel);
uint8_t __attribute__((optimize("O0")))Check_Software_Timers(void);

#endif /* INC_TIMERS_H_ */
