#ifndef INC_INPUT_CAPTURE_H_
#define INC_INPUT_CAPTURE_H_

//MACROS
#define INPUT_CAPTURE_PROCESSING_CAN_BE_STARTED (0b1 << 2)
#define SOFTWARE_IP_CAP_IDLE_TIMER_HAS_TIMED_OUT (0b1 << 5)
#define SOFTWARE_IP_CAP_IDLE_TIMER_IS_RUNNING (0b1 << 6)

//INCLUDES
#include <stdint.h>
#include "main.h"
#include "config.h"
#include "oscillator_params_type.h"
#include "oscillator.h"
#include "utility.h"

//VARIABLE DECLARATIONS
volatile extern uint32_t TIM2_ch1_input_capture_value;
volatile extern uint16_t interrupt_period;

//CUSTOM TYPES


//FUNCTION DECLARATIONS
uint8_t Input_Capture_Processing(volatile uint16_t interrupt_period_value, struct Params* params_ptr);
uint8_t Start_Input_Capture_Timer(void);
uint8_t Start_IC_TIM(TIM_HandleTypeDef *TIM, uint32_t IC_TIM_channel);
uint8_t Copy_Params_Structs(struct Params* src_ptr, struct Params* dst_ptr);
uint8_t Start_Measurement_Reelapse_Timer(void);
uint8_t Begin_Input_Capture_Measurement(void);

#endif /* INC_INPUT_CAPTURE_H_ */
