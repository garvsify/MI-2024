#ifndef INC_OSCILLATOR_H_
#define INC_OSCILLATOR_H_

//INCLUDES
#include <stdint.h>

#include "utility.h"
#include "symmetry_adjustment.h"
#include "oscillator_params_type.h"
#include "config.h"
#include "timers.h"
#include "oscillator_macros.h"

//MACROS

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
		//TURN ON/OFF SYMMETRY and SET RESOLUTION
		//set SYMMETRY_ADC_RESOLUTION
		#define SYMMETRY_ON_OR_OFF ON

		//TURN ON/OFF DEPTH
		#define DEPTH_ON_OR_OFF ON

		//TURN TAP/TEMPO AND CLK_IN ON/OFF
		#define TAPCLK_ON_OR_OFF ON
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


#define SPEED_ADC_RESOLUTION 10 //- even if resolutions are truncated, maths in the code needs these definitions
#define DEPTH_ADC_RESOLUTION 7 //- even if resolutions are truncated, maths in the code needs these definitions
#define WAVESHAPE_ADC_RESOLUTION 12 //- even if resolutions are truncated, maths in the code needs these definitions
#define DELAY_ADC_RESOLUTION 9 //- even if resolutions are truncated, maths in the code needs these definitions

#define SYMMETRY_ADC_RESOLUTION 8 //- even if resolutions are truncated, maths in the code needs these definitions

#if SYMMETRY_ADC_RESOLUTION == 8
	#define SYMMETRY_ADC_HALF_SCALE_NO_BITS 7
	#define SYMMETRY_ADC_FULL_SCALE 255
	#define SYMMETRY_ADC_HALF_SCALE 128
	#define SYMMETRY_ADC_NUM_BITS 8
#endif

#define START_REQUIRED_BEFORE_SYNC_MODE (0b1 << 7)


//CONSTANTS
extern const uint16_t sine_wavetable[512];
extern const uint16_t tri_wavetable[512];
extern const uint16_t TIM16_prescalers[6];

//VARIABLES
volatile extern uint16_t ADCResultsDMA[5];
const extern uint8_t num_ADC_conversions;

//STRUCT VARIABLES
extern struct Params params; //running variables of the oscillator/s
extern struct Params params_to_be_loaded; //'to be loaded' params for the oscillators for when I/P capture states are active
extern struct Params params_working; //upon overflow of re-elapse timer (TIM3), 'to_be_loaded' values are copied into this struct
extern struct Delay_Line delay_line;

//CUSTOM TYPES
enum Next_Values_Processing_Mode{
	REGULAR_MODE,
	IP_CAPTURE_MODE,
	STARTUP_MODE
};

struct Delay_Line{

	volatile uint16_t duty_delay_line_storage_array[513]; //one index larger than the number of indexes (wave samples) to allow us to 'wrap' the array into a kind of circular buffer
	volatile uint16_t duty_delay_line_start_offset; //initial value is 1st index
	volatile uint16_t duty_delay_line_finish_offset; //initial value is 512th index (513th value)
	volatile uint16_t duty_delay_line_read_pointer_offset;
};

//FUNCTION DECLARATIONS
uint8_t Start_PWM_Gen_Timer_Main_and_Secondary_Oscillators(TIM_HandleTypeDef *TIM, uint32_t PWM_TIM_channel_1, uint32_t PWM_TIM_channel_2);
uint8_t Start_Freq_Gen_Timer(void);

uint8_t Set_Oscillator_Values(struct Params* params_ptr);
uint8_t Calculate_Next_Main_Oscillator_Values(struct Params* params_ptr, enum Next_Values_Processing_Mode mode);
uint8_t Write_Next_Main_Oscillator_Values_to_Delay_Line(struct Params* params_ptr, struct Delay_Line* delay_line_ptr);
uint8_t Process_ADC_Conversion_Values(struct Params* params_ptr, struct Delay_Line* delay_line_ptr, volatile uint16_t* ADCResultsDMA_ptr);
uint8_t Process_TIM16_Raw_Start_Value_and_Raw_Prescaler(struct Params* params_ptr);

#endif
