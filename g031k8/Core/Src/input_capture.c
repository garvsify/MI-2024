#include "input_capture.h"

//VARIABLE DEFINITIONS
volatile enum Input_Capture_Event input_capture_event = FIRST;
volatile uint32_t TIM2_ch1_input_capture_value;
volatile enum Validate input_capture_measurement_is_ongoing = NO;
volatile enum Validate input_capture_measurement_reelapse_is_ongoing = NO;
volatile enum Validate input_capture_processing_can_be_started = NO;

volatile uint16_t interrupt_period = 0;

//FUNCTION DEFINITIONS
uint8_t Input_Capture_Processing(volatile uint16_t interrupt_period_value, struct Params* params_ptr){

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

	input_capture_processing_can_be_started = NO; //reset flag

	//DETERMINE WHAT TO SET THE RAW_START_VALUE AND BASE_PRESCALER TO BASED ON THE I/P CAPTURE VALUE
	//CHECK FOR PRIMALITY
	if(isPrime(interrupt_period_value) == YES){

		interrupt_period_value += 1;
	}

	//START FINDING FACTORS
	uint32_t N = interrupt_period_value << 6; //calculate the N-value which is prescaler_meas * interrupt_period_meas. The measurement prescaler is used which is 64. (TIM2 has a prescaler of 64*512, but since we divide this value by 512, the prescaler is then just 64).

	for(uint8_t i = 0; i < 129; i++){ //check from period = 264 to 128 - there will be a prescaler for every non-prime value of N

		interrupt_period_value = 256 - i;
		uint16_t remainder = N % interrupt_period_value;

		if(remainder == 0){ //check if no remainder -> integer

			params_ptr->raw_prescaler = N / interrupt_period_value;
			break;
		}
	}

	params_ptr->raw_start_value = 256 - interrupt_period_value;

	Calculate_Next_Main_Oscillator_Values(params_ptr, (enum Next_Values_Processing_Mode)IP_CAPTURE_MODE);
	Process_TIM16_Final_Start_Value_and_Final_Prescaler(params_ptr);

	//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);

	return 1;
}

