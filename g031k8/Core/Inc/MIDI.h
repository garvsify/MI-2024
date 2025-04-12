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

struct MIDI_Data{

	uint8_t MIDI_data_buffer[10];
};

//VARIABLE DECLARATIONS
volatile extern enum MIDI_Channel MIDI_basic_channel;
volatile extern uint8_t active_status_byte;
volatile extern uint8_t running_status_byte;
volatile extern struct MIDI_Data MIDI_data = {.MIDI_data_buffer = 0};

//FUNCTION DECLARATIONS
enum Validate Is_Status_Byte(volatile char *data);
enum Validate Is_Data_Byte(volatile char *data);
enum Validate Is_PC_Status_Byte(volatile char *data);
enum Validate Is_CC_Status_Byte(volatile char *data);
enum Validate Is_Sysex_Start_Status_Byte(volatile char *data);
enum Validate Is_Sysex_End_Status_Byte(volatile char *data);
enum Validate Is_Channel_Status_Byte_On_Basic_Channel(volatile char *data, volatile enum MIDI_Channel MIDI_basic_channel);
enum Validate Is_Data_Buffer_Empty(struct MIDI_Data *MIDI_data_struct);
uint8_t Num_Data_Bytes_Received(struct MIDI_Data *MIDI_data_struct);
enum Validate Is_Program_Change_Data_Byte_In_Range(volatile char *PC_data, uint8_t size_of_factory_or_user_array);
enum Validate Is_System_Real_Time_Status_Byte(volatile char *data);
enum Validate Is_OMNI_On(volatile uint32_t *statuses_ptr);

#endif /* SRC_MIDI_H_ */
