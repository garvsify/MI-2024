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
#define NUMBER_OF_FREQUENCY_STEPS 625; //625 is the max. possible if prescaler array is limited to index 5
#define FIRST_HALFCYCLE 0
#define SECOND_HALFCYCLE 1
#define FIRST_QUADRANT 0
#define SECOND_QUADRANT 1
#define TRIANGLE_MODE_ADC_THRESHOLD 1365
#define SINE_MODE_ADC_THRESHOLD 2730
#define SQUARE_MODE_ADC_THRESHOLD 4095
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
#define SIXTY_FOUR_PRESCALER_LARGEST_INTERRUPT_PERIOD 256
#define ONE_HUNDRED_TWENTY_EIGHT_PRESCALER_LARGEST_INTERRUPT_PERIOD 512
#define TWO_HUNDRED_FIFTY_SIX_PRESCALER_LARGEST_INTERRUPT_PERIOD 1024
#define FIVE_HUNDRED_TWELVE_PRESCALER_LARGEST_INTERRUPT_PERIOD 2048
#define ONE_THOUSAND_TWENTY_FOUR_PRESCALER_LARGEST_INTERRUPT_PERIOD 4096
#define SLOWEST_SPEED_PRESCALER_DIVISORS_ARRAY_INDEX 1
#define FASTEST_SPEED_PRESCALER_DIVISORS_ARRAY_INDEX 5
#define SPEED_TOLERANCE 2
#define INITIAL_PWM_VALUE 512
//#define LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK 4001 - 1 //8ms overflow with prescaler of 128
#define LPTIM1_CCR_TAP_TEMPO_SW_IN_CHECK 2001 - 1 //8ms overflow with prescaler of 128
#define TAP_TEMPO_SWITCH_CONFIDENCE_COUNT 5
//#define TIM17_DEBOUNCE_LENGTH 55
#define SPEED_POT_CHECK_LENGTH 25
#define COUNT_TO_DELAY_RISING_TAP_TEMPO_EDGE 1


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

		#if SYMMETRY_ADC_RESOLUTION == 10
			#define SYMMETRY_ADC_HALF_SCALE_NO_BITS 9
			#define SYMMETRY_ADC_FULL_SCALE 1023
			#define SYMMETRY_ADC_HALF_SCALE 511
			#define SYMMETRY_ADC_NUM_BITS 10
		#endif

		#if SYMMETRY_ADC_RESOLUTION == 12
			#define SYMMETRY_ADC_FULL_SCALE 4095
			#define SYMMETRY_ADC_HALF_SCALE_NO_BITS 11
			#define SYMMETRY_ADC_HALF_SCALE 2048
			#define SYMMETRY_ADC_NUM_BITS 12
		#endif

		//TURN ON/OFF DEPTH
		#define DEPTH_ON_OR_OFF ON

//CONSTANTS
extern const uint16_t sine_wavetable[512];
extern const uint16_t tri_wavetable[512];
extern const uint16_t TIM16_prescaler_divisors[6];

//VARIABLES
volatile extern uint16_t duty;
volatile extern uint8_t current_waveshape;
volatile extern uint16_t current_speed;
volatile extern uint16_t current_depth;
volatile extern uint32_t current_symmetry;
volatile extern uint16_t current_index;
volatile extern uint8_t current_halfcycle;
volatile extern uint8_t current_quadrant;
volatile extern uint16_t ADCResultsDMA[4];
const extern uint8_t num_ADC_conversions;
volatile extern enum Validate initial_ADC_conversion_complete;
volatile extern enum Validate TIM16_callback_active;
volatile extern uint16_t TIM16_raw_start_value;
volatile extern uint16_t TIM16_final_start_value;
volatile extern uint8_t TIM16_base_prescaler_divisors_index;
volatile extern enum Adjust_Prescaler_Action TIM16_prescaler_adjust;
volatile extern uint16_t TIM16_final_start_value_locked;
volatile extern uint8_t TIM16_prescaler_divisors_final_index_locked;
volatile extern uint8_t TIM16_prescaler_divisors_final_index;
volatile extern uint16_t prev_duty;
volatile extern enum Input_Capture_Event input_capture_event;
volatile extern uint32_t TIM2_ch1_input_capture_value;
volatile extern enum Validate input_capture_measurement_is_ongoing;
volatile extern enum Validate speed_pot_is_disabled;
volatile extern enum Validate input_capture_measurement_reelapse_is_ongoing;
volatile extern uint16_t interrupt_period;
volatile extern enum Adjust_Prescaler_Action TIM16_prescaler_adjust_to_be_loaded;
volatile extern uint16_t TIM16_raw_start_value_to_be_loaded;
volatile extern uint16_t TIM16_final_start_value_to_be_loaded;
volatile extern uint8_t TIM16_base_prescaler_divisors_index_to_be_loaded;
volatile extern uint16_t duty_to_be_loaded;
volatile extern uint8_t TIM16_prescaler_divisors_final_index_to_be_loaded;
volatile extern uint8_t current_halfcycle_to_be_loaded;
volatile extern uint8_t current_quadrant_to_be_loaded;
volatile extern uint16_t current_index_to_be_loaded;
volatile extern uint16_t current_depth_to_be_loaded;
volatile extern uint16_t duty_delay_line_storage_array[513]; //one index larger than the number of indexes (wave samples) to allow us to 'wrap' the array into a kind of circular buffer
volatile extern uint16_t duty_delay_line_start_offset; //initial value is 1st index
volatile extern uint16_t duty_delay_line_finish_offset; //initial value is 512th index (513th value)
volatile extern uint16_t duty_delay_line_read_pointer_offset;
volatile extern uint16_t duty_delayed;
volatile extern enum Validate TAP_TEMPO_EXTI4_15_IRQ_is_disabled;
volatile extern enum Validate tap_tempo_mode_is_active;
volatile extern uint8_t speed_pot_adc_measurement_num;
volatile extern enum Validate is_very_first_oscillation;
volatile extern enum Validate UART_DMA_TX_is_complete;
volatile extern char rx_buffer[1];
extern uint8_t tap_tempo_switch_state_counter;
extern enum Tap_Tempo_Switch_State tap_tempo_switch_state;
volatile extern enum Validate TIM17_debounce_is_elapsing;

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
uint8_t Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(void);
uint8_t Process_TIM16_Final_Start_Value_and_Prescaler_Adjust(void);
uint8_t Adjust_and_Set_TIM16_Prescaler(uint8_t TIM16_prescaler_adjust_arg);
uint32_t unsigned_bitwise_modulo(uint32_t dividend, uint8_t base_2_exponent);
uint8_t Speed_pot_check(void);
uint8_t Check_Tap_Tempo_Switch_State(enum Tap_Tempo_Switch_State *tap_tempo_switch_state_ptr);
uint8_t Start_Tap_Tempo_Monitoring_Timers_and_UART_Receive(void);

#endif
