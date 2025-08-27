#ifndef INC_TIMERS_H_
#define INC_TIMERS_H_

//DEFINES
#define HTIM17_DMA_M2M_DELAY 2000

//INCLUDES
#include <stdint.h>

#include "config.h"

//FUNCTION DECLARATIONS
uint8_t Start_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel);
uint8_t Stop_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel);
uint8_t Start_DMA_M2M_Timer(void);

#endif /* INC_TIMERS_H_ */
