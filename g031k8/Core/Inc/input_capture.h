#ifndef INC_INPUT_CAPTURE_H_
#define INC_INPUT_CAPTURE_H_

//MACROS
#define INPUT_CAPTURE_MEASUREMENT_IS_ONGOING (0b1 << 0)
#define INPUT_CAPTURE_MEASUREMENT_REELAPSE_IS_ONGOING (0b1 << 1)
#define INPUT_CAPTURE_PROCESSING_CAN_BE_STARTED (0b1 << 2)
#define IP_CAP_EVENTS_DETECTION_TIMEOUT (0b1 << 5)

//INCLUDES
#include <stdint.h>
#include "main.h"
#include "config.h"
#include "oscillator_params_type.h"
#include "oscillator.h"
#include "utility.h"

//VARIABLE DECLARATIONS
volatile extern enum Input_Capture_Event input_capture_event;
volatile extern uint32_t TIM2_ch1_input_capture_value;
volatile extern uint16_t interrupt_period;

//CUSTOM TYPES
enum Input_Capture_Event{
	FIRST,
	SECOND
};

//FUNCTION DECLARATIONS
uint8_t Input_Capture_Processing(volatile uint16_t interrupt_period_value, struct Params* params_ptr);
uint8_t Start_Input_Capture_Timer(void);
uint8_t Start_IC_TIM(TIM_HandleTypeDef *TIM, uint32_t IC_TIM_channel);
uint8_t Copy_Params_Structs(struct Params* src_ptr, struct Params* dst_ptr);

#endif /* INC_INPUT_CAPTURE_H_ */
