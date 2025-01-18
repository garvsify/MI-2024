#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

//INCLUDES
#include "config.h"
#include <stdint.h>

//MACROS
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
#define TRIANGLE_MODE_ADC_THRESHOLD 1365
#define SINE_MODE_ADC_THRESHOLD 2730
#define SQUARE_MODE_ADC_THRESHOLD 4095
#define WAVESHAPE_ADC_RESULT_ARRAY_POS 0
#define SPEED_ADC_RESULT_ARRAY_POS 1
#define DEPTH_ADC_RESULT_ARRAY_POS 2
#define SYMMETRY_ADC_RESULT_ARRAY_POS 3
#define DUTY_DELAY_LINE_READ_POINTER_OFFSET_ADC_RESULT_ARRAY_POS 4
#define ON 1
#define OFF 0
#define SINE_OR_TRIANGLE_WAVE_PREEMPTIVE_DELAY 50
#define SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX 384 - SINE_OR_TRIANGLE_WAVE_PREEMPTIVE_DELAY
#define SQUARE_WAVE_TEMPO_APEX_INDEX 256
#define TEMPO_PULSE_LENGTH 32
#define SINE_OR_TRIANGLE_WAVE_TEMPO_PULSE_OFF_INDEX SINE_OR_TRIANGLE_WAVE_TEMPO_PERCEIVED_APEX_INDEX + TEMPO_PULSE_LENGTH
#define SQUARE_WAVE_TEMPO_PULSE_OFF_INDEX SQUARE_WAVE_TEMPO_APEX_INDEX + TEMPO_PULSE_LENGTH
#define HIGHEST_PRESCALER_TOP_SPEED_PERIOD 129
#define PRESCALER_DIVISORS_MAX_INDEX 5
#define SLOWEST_SPEED_PRESCALER 1024
#define SPEED_TOLERANCE 3
#define INITIAL_PWM_VALUE 512
//#define LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK 4001 - 1 //4ms overflow with prescaler of 128
#define LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK 1001 - 1 //2ms overflow with prescaler of 128
#define TAP_TEMPO_SWITCH_CONFIDENCE_COUNT 5
#define COUNT_TO_DELAY_RISING_TAP_TEMPO_EDGE 1
#define SPEED_POT_CHECK_COUNTER_LENGTH 25


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
		//set SYMMETRY_ADC_RESOLUTION to either 8, 10, or 12
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
		#define DEPTH_ADC_RESOLUTION 8
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
volatile extern enum Input_Capture_Event input_capture_event;
volatile extern uint32_t TIM2_ch1_input_capture_value;
volatile extern enum Validate input_capture_measurement_is_ongoing;
volatile extern enum Validate input_capture_measurement_reelapse_is_ongoing;
volatile extern uint16_t interrupt_period;
volatile extern uint8_t speed_pot_adc_measurement_num;
volatile extern enum Validate is_very_first_oscillation;
volatile extern enum Validate UART_DMA_TX_is_complete;
volatile extern char rx_buffer[1];
extern uint8_t tap_tempo_switch_state_counter;
extern enum Tap_Tempo_Switch_State tap_tempo_switch_state;
volatile extern enum Validate input_capture_processing_can_be_started;
volatile extern enum Validate external_clock_mode_is_active;
volatile extern enum Validate tap_tempo_mode_is_active;

//STRUCT VARIABLES
extern struct Params params; //'normal' running variables of the oscillator/s
extern struct Params params_to_be_loaded; //'to be loaded' variables for the oscillators for I/P capture purposes
extern struct Delay_Line delay_line;

//CUSTOM TYPES
enum Polarity{
	NEGATIVE,
	POSITIVE
};

enum Adjust_Prescaler_Action{
	DO_NOTHING,
	MULTIPLY_BY_TWO,
};

enum Validate{
	NO,
	YES
};

enum TIM16_final_start_value_Oscillation_Mode{
	DO_NOT_OSCILLATE,
	OSCILLATE_UPWARDS,
	OSCILLATE_DOWNWARDS
};

enum Symmetry_Status{
	CW,
	CCW
};

enum Symmetry_Type{
	SHORTEN,
	LENGTHEN
};

enum Input_Capture_Event{
	FIRST,
	SECOND
};

enum Tap_Tempo_Switch_State{
	NOT_DEPRESSED,
	DEPRESSED
};

struct Params{

	volatile uint16_t duty;
	volatile uint16_t prev_duty;
	volatile uint16_t duty_delayed;
	volatile uint8_t waveshape;
	volatile uint16_t speed;
	volatile uint8_t depth;
	volatile uint16_t symmetry;
	volatile uint16_t index;
	volatile uint8_t halfcycle;
	volatile uint8_t quadrant;
	volatile uint16_t raw_start_value;
	volatile uint16_t final_start_value;
	volatile uint16_t raw_prescaler;
	volatile uint16_t final_prescaler;
	volatile enum Adjust_Prescaler_Action prescaler_adjust;
};

struct Delay_Line{

	volatile uint16_t duty_delay_line_storage_array[513]; //one index larger than the number of indexes (wave samples) to allow us to 'wrap' the array into a kind of circular buffer
	volatile uint16_t duty_delay_line_start_offset; //initial value is 1st index
	volatile uint16_t duty_delay_line_finish_offset; //initial value is 512th index (513th value)
	volatile uint16_t duty_delay_line_read_pointer_offset;
};

enum Next_Values_Processing_Mode{
	REGULAR_MODE,
	IP_CAPTURE_MODE
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

uint8_t Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(uint8_t adc_resolution_bits, uint16_t speed_range, struct Params* params_ptr);
uint8_t Process_TIM16_Final_Start_Value_and_Final_Prescaler(struct Params* params_ptr);
uint8_t Adjust_TIM16_Prescaler(struct Params* params_ptr);

uint32_t unsigned_bitwise_modulo(uint32_t dividend, uint8_t base_2_exponent);

uint8_t Speed_Pot_Check(struct Params* params_ptr);
uint8_t Check_Tap_Tempo_Switch_State(enum Tap_Tempo_Switch_State *tap_tempo_switch_state_ptr);
uint8_t Start_UART_Receive(void);

enum Validate isPrime(uint16_t x);
uint8_t Input_Capture_Processing(volatile uint16_t interrupt_period_value, struct Params* params_ptr);

uint8_t Calculate_Next_Main_Oscillator_Values(struct Params* params_ptr, enum Next_Values_Processing_Mode mode);
uint8_t Write_Next_Main_Oscillator_Values_to_Delay_Line(struct Params* params_ptr, struct Delay_Line* delay_line_ptr);
uint8_t Process_ADC_Conversion_Values(struct Params* params_ptr, struct Delay_Line* delay_line_ptr, volatile uint16_t* ADCResultsDMA_ptr);

uint8_t Set_Oscillator_Values(struct Params* params_ptr);

#endif
