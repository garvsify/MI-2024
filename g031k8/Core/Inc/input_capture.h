#ifndef INC_INPUT_CAPTURE_H_
#define INC_INPUT_CAPTURE_H_

//INCLUDES
#include <stdint.h>
#include "oscillator.h"

//VARIABLE DECLARATIONS
volatile extern enum Input_Capture_Event input_capture_event;
volatile extern uint32_t TIM2_ch1_input_capture_value;
volatile extern enum Validate input_capture_measurement_is_ongoing;
volatile extern enum Validate input_capture_measurement_reelapse_is_ongoing;
volatile extern enum Validate input_capture_processing_can_be_started;

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

#endif /* INC_INPUT_CAPTURE_H_ */
