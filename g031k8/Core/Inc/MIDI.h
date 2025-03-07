#ifndef SRC_MIDI_H_
#define SRC_MIDI_H_

//MACROS
#define MIDI_BASIC_CHANNEL_DEFAULT 0 //CH1

//INCLUDES
#include <stdint.h>
#include "utility.h"

//CUSTOM TYPES
enum MIDI_Channel{
	MIDI_CH_ONE,
	MIDI_CH_TWO,
	MIDI_CH_THREE,
	MIDI_CH_FOUR,
	MIDI_CH_FIVE,
	MIDI_CH_SIX,
	MIDI_CH_SEVEN,
	MIDI_CH_EIGHT,
	MIDI_CH_NINE,
	MIDI_CH_TEN,
	MIDI_CH_ELEVEN,
	MIDI_CH_TWELVE,
	MIDI_CH_THIRTEEN,
	MIDI_CH_FOURTEEN,
	MIDI_CH_FIFTEEN,
	MIDI_CH_SIXTEEN,
};

//VARIABLE DECLARATIONS
volatile extern enum MIDI_Channel MIDI_basic_channel;

//FUNCTION DECLARATIONS
enum Validate Is_Status_Byte(volatile char *data);
enum Validate Is_Program_Change_Data_Byte_In_Range(volatile char *PC_data, uint8_t size_of_factory_or_user_array);
enum Validate Is_System_Real_Time_Status_Byte(volatile char *data);

#endif /* SRC_MIDI_H_ */
