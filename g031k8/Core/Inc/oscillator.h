#ifndef INC_OSCILLATOR_H_
#define INC_OSCILLATOR_H_

//INCLUDES
#include "utility.h"
#include "symmetry_adjustment.h"
#include "oscillator_params_type.h"
#include "config.h"

#include <stdint.h>

//MACROS
#define INITIAL_PWM_VALUE 512
#define PWM_DUTY_VALUE_MAX 1023
#define PWM_DUTY_VALUE_MIN 0

#define FIRST_QUADRANT_START_INDEX 0
#define SECOND_QUADRANT_START_INDEX 128
#define THIRD_QUADRANT_START_INDEX 256
#define FOURTH_QUADRANT_START_INDEX 384
#define FINAL_INDEX 511
#define TRIANGLE_MODE 0
#define SINE_MODE 1
#define SQUARE_MODE 2
#define NUMBER_OF_FREQUENCY_STEPS 660
#define FIRST_HALFCYCLE 0
#define SECOND_HALFCYCLE 1
#define FIRST_QUADRANT 0
#define SECOND_QUADRANT 1

#define TRIANGLE_MODE_ADC_THRESHOLD 1365 >> 5
#define SINE_MODE_ADC_THRESHOLD 2730 >> 5
#define SQUARE_MODE_ADC_THRESHOLD 4095 >> 5
#define WAVESHAPE_ADC_RESULT_INDEX 0
#define SPEED_ADC_RESULT_INDEX 1
#define DEPTH_ADC_RESULT_INDEX 2
#define SYMMETRY_ADC_RESULT_INDEX 3
#define DUTY_DELAY_LINE_READ_POINTER_OFFSET_ADC_RESULT_INDEX 4

#define SINE_OR_TRIANGLE_WAVE_PREEMPTIVE_DELAY 50
#define SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX 384 - SINE_OR_TRIANGLE_WAVE_PREEMPTIVE_DELAY
#define SQUARE_WAVE_TEMPO_APEX_INDEX 256
#define TEMPO_PULSE_LENGTH 32
#define SINE_OR_TRIANGLE_WAVE_TEMPO_PULSE_OFF_INDEX SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX + TEMPO_PULSE_LENGTH
#define SQUARE_WAVE_TEMPO_PULSE_OFF_INDEX SQUARE_WAVE_TEMPO_APEX_INDEX + TEMPO_PULSE_LENGTH

#define HIGHEST_PRESCALER_TOP_SPEED_PERIOD 129
#define PRESCALER_DIVISORS_MAX_INDEX 5
#define SLOWEST_SPEED_PRESCALER 1024


#if SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX < SECOND_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SINE_OR_TRI_SYNCED = FIRST_QUADRANT
	#define CURRENT_HALFCYCLE_SINE_OR_TRI_SYNCED = FIRST_HALFCYCLE

#elif SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX < THIRD_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SINE_OR_TRI_SYNCED = SECOND_QUADRANT
	#define CURRENT_HALFCYCLE_SINE_OR_TRI_SYNCED = FIRST_HALFCYCLE

#elif SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX < FOURTH_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SINE_OR_TRI_SYNCED FIRST_QUADRANT
	#define CURRENT_HALFCYCLE_SINE_OR_TRI_SYNCED SECOND_HALFCYCLE

#elif SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX <= FINAL_INDEX
	#define CURRENT_QUADRANT_SINE_OR_TRI_SYNCED = SECOND_QUADRANT
	#define CURRENT_HALFCYCLE_SINE_OR_TRI_SYNCED = SECOND_HALFCYCLE
#endif


#if SQUARE_WAVE_TEMPO_APEX_INDEX < SECOND_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SQUARE_SYNCED = FIRST_QUADRANT
	#define CURRENT_HALFCYCLE_SQUARE_SYNCED = FIRST_HALFCYCLE

#elif SQUARE_WAVE_TEMPO_APEX_INDEX < THIRD_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SQUARE_SYNCED = SECOND_QUADRANT
	#define CURRENT_HALFCYCLE_SQUARE_SYNCED = FIRST_HALFCYCLE

#elif SQUARE_WAVE_TEMPO_APEX_INDEX < FOURTH_QUADRANT_START_INDEX
	#define CURRENT_QUADRANT_SQUARE_SYNCED FIRST_QUADRANT
	#define CURRENT_HALFCYCLE_SQUARE_SYNCED SECOND_HALFCYCLE

#elif SQUARE_WAVE_TEMPO_APEX_INDEX <= FINAL_INDEX
	#define CURRENT_QUADRANT_SQUARE_SYNCED = SECOND_QUADRANT
	#define CURRENT_HALFCYCLE_SQUARE_SYNCED = SECOND_HALFCYCLE

#endif


		//TURN ON/OFF SYMMETRY and SET RESOLUTION
		//set SYMMETRY_ADC_RESOLUTION
		#define SYMMETRY_ADC_RESOLUTION 8
		#define SYMMETRY_ON_OR_OFF ON

		#if SYMMETRY_ADC_RESOLUTION == 8
			#define SYMMETRY_ADC_HALF_SCALE_NO_BITS 7
			#define SYMMETRY_ADC_FULL_SCALE 255
			#define SYMMETRY_ADC_HALF_SCALE 128
			#define SYMMETRY_ADC_NUM_BITS 8
		#endif

		//TURN ON/OFF DEPTH
		#define DEPTH_ON_OR_OFF ON

		#define SPEED_ADC_RESOLUTION 10
		#define DEPTH_ADC_RESOLUTION 7
		#define WAVESHAPE_ADC_RESOLUTION 12
		#define DELAY_ADC_RESOLUTION 9


//CONSTANTS
extern const uint16_t sine_wavetable[512];
extern const uint16_t tri_wavetable[512];
extern const uint16_t TIM16_prescalers[6];

//VARIABLES
volatile extern uint16_t ADCResultsDMA[5];
const extern uint8_t num_ADC_conversions;
volatile extern enum Validate initial_ADC_conversion_complete;

//STRUCT VARIABLES
extern struct Params params; //'normal' running variables of the oscillator/s
extern struct Params params_to_be_loaded; //'to be loaded' variables for the oscillators for I/P capture purposes
extern struct Delay_Line delay_line;

//CUSTOM TYPES
enum Next_Values_Processing_Mode{
	REGULAR_MODE,
	IP_CAPTURE_MODE
};

struct Delay_Line{

	volatile uint16_t duty_delay_line_storage_array[513]; //one index larger than the number of indexes (wave samples) to allow us to 'wrap' the array into a kind of circular buffer
	volatile uint16_t duty_delay_line_start_offset; //initial value is 1st index
	volatile uint16_t duty_delay_line_finish_offset; //initial value is 512th index (513th value)
	volatile uint16_t duty_delay_line_read_pointer_offset;
};

//FUNCTION DECLARATIONS
uint8_t Global_Interrupt_Enable(void);
uint8_t Global_Interrupt_Disable(void);
uint8_t Startup(void);
uint8_t Start_PWM_Gen_Timer_Main_and_Secondary_Oscillators(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel_1, uint32_t PWM_TIM_channel_2);
uint8_t Start_Freq_Gen_Timer(void);
uint8_t Start_Input_Capture_Timer(void);
uint8_t Start_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel);
uint8_t Stop_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel);
uint8_t Start_IC_TIM(TIM_HandleTypeDef *TIM, uint32_t IC_TIM_channel);

uint8_t Set_Oscillator_Values(struct Params* params_ptr);
uint8_t Calculate_Next_Main_Oscillator_Values(struct Params* params_ptr, enum Next_Values_Processing_Mode mode);
uint8_t Write_Next_Main_Oscillator_Values_to_Delay_Line(struct Params* params_ptr, struct Delay_Line* delay_line_ptr);
uint8_t Process_ADC_Conversion_Values(struct Params* params_ptr, struct Delay_Line* delay_line_ptr, volatile uint16_t* ADCResultsDMA_ptr);
uint8_t Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(struct Params* params_ptr);

#endif
