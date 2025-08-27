#ifndef SRC_UTILITY_H_
#define SRC_UTILITY_H_

//MACROS
#define ON 1
#define OFF 0

#define START_REQUIRED_BEFORE_SYNC_MODE (0b1 << 7)

//INCLUDES
#include <stdint.h>
#include "input_capture.h"
#include "config.h"
#include "oscillator_macros.h"
#include "MIDI_defines.h"
#include "checking_functions.h"
#include "tap_tempo.h"

//CUSTOM TYPES
enum Polarity{
	NEGATIVE,
	POSITIVE
};

enum Validate{
	NO,
	YES
};

enum Status_Bit{
	Input_Capture_Processing_Can_Be_Started = INPUT_CAPTURE_PROCESSING_CAN_BE_STARTED,
	First_Sync_Complete = FIRST_SYNC_COMPLETE,
	Initial_ADC_Conversion_Complete = INITIAL_ADC_CONVERSION_COMPLETE,
	Software_IP_CAP_Idle_Timer_Has_Timed_Out = SOFTWARE_IP_CAP_IDLE_TIMER_HAS_TIMED_OUT,
	Software_IP_CAP_Idle_Timer_Is_Running = SOFTWARE_IP_CAP_IDLE_TIMER_IS_RUNNING,
	Start_Required_Before_Sync_Mode = START_REQUIRED_BEFORE_SYNC_MODE,
	Software_MIDI_Timer_Is_Running = SOFTWARE_MIDI_TIMER_IS_RUNNING,
	Software_MIDI_Timer_Has_Timed_Out = SOFTWARE_MIDI_TIMER_HAS_TIMED_OUT,
	MIDI_Channel_Voice_Mode = MIDI_CHANNEL_VOICE_MODE,
	Pots_Counter_Has_Timed_Out = POTS_COUNTER_HAS_TIMED_OUT,
	Tap_Tempo_Preset_Save_Timer_Is_Running = TAP_TEMPO_PRESET_SAVE_TIMER_IS_RUNNING,
	Tap_Tempo_Preset_Save_Timer_Has_Timed_Out = TAP_TEMPO_PRESET_SAVE_TIMER_HAS_TIMED_OUT,
	Processing_Mutex = PROCESSING_MUTEX,
};

//FUNCTION DECLARATIONS
enum Validate __attribute__((optimize("O0")))isPrime(uint16_t x);
uint32_t __attribute__((optimize("O0")))unsigned_bitwise_modulo(uint32_t dividend, uint8_t base_2_exponent);
uint8_t __attribute__((optimize("O0")))Global_Interrupt_Enable(void);
uint8_t __attribute__((optimize("O0")))Global_Interrupt_Disable(void);
enum Validate __attribute__((optimize("O0")))Get_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit);
void __attribute__((optimize("O0")))Set_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit);
void __attribute__((optimize("O0")))Clear_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit);

#endif /* SRC_UTILITY_H_ */
