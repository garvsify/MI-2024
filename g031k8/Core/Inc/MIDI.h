#ifndef SRC_MIDI_H_
#define SRC_MIDI_H_

//MACROS
#define MIDI_BASIC_CHANNEL_DEFAULT 0 //CH1

#define CHANNEL_MODE_CC_THRESHOLD 120

//channel mode first data byte defines
//#define ALL_SOUND_OFF 120
#define RESET_ALL_CONTROLLERS 121
#define LOCAL_CONTROL 122
//#define ALL_NOTES_OFF 123
#define OMNI_MODE_OFF 124
#define OMNI_MODE_ON 125
//#define MONO_MODE_ON 126
//#define POLY_MODE_ON 127

#define LOCAL_CONTROL_OFF 0
#define LOCAL_CONTROL_ON 127

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

	volatile uint8_t MIDI_data_buffer[10];
};

//VARIABLE DECLARATIONS
volatile extern enum MIDI_Channel MIDI_basic_channel;
volatile extern uint8_t active_status_byte;
volatile extern uint8_t running_status_byte;
volatile extern struct MIDI_Data MIDI_data;

//FUNCTION DECLARATIONS
enum Validate Is_Status_Byte(volatile uint8_t *data);
enum Validate Is_Data_Byte(volatile uint8_t *data);
enum Validate Is_PC_Status_Byte(volatile uint8_t *data);
enum Validate Is_CC_Status_Byte(volatile uint8_t *data);
enum Validate Is_Utilised_Channel_Mode_CC_First_Data_Byte(volatile uint8_t *first_data_byte);
enum Validate Is_Utilised_CC_First_Data_Byte(volatile uint8_t *first_data_byte);
enum Validate Channel_Mode_CC_Second_Data_Byte_Is_Valid_Given_Utilised_First_Data_Byte(volatile uint8_t *first_data_byte, volatile uint8_t *second_data_byte);
uint8_t Process_Channel_Mode_Message(volatile struct MIDI_Data *MIDI_data_ptr, uint32_t *statuses_ptr);
enum Validate Is_Sysex_Start_Status_Byte(volatile uint8_t *data);
enum Validate Is_Sysex_End_Status_Byte(volatile uint8_t *data);
enum Validate Is_Channel_Status_Byte_On_Basic_Channel(volatile uint8_t *data, volatile enum MIDI_Channel MIDI_basic_channel);
enum Validate Is_Data_Buffer_Empty(volatile struct MIDI_Data *MIDI_data_struct);
uint8_t Num_Data_Bytes_Received(volatile struct MIDI_Data *MIDI_data_struct);
uint8_t Clear_Data_Buffer(volatile struct MIDI_Data *MIDI_data_struct);
enum Validate Is_Program_Change_Data_Byte_In_Range(volatile uint8_t *PC_data, uint8_t size_of_factory_or_user_array);
enum Validate Is_System_Real_Time_Status_Byte(volatile uint8_t *data);
enum Validate Is_OMNI_On(volatile uint32_t *statuses_ptr);
uint8_t Reset_and_Stop_MIDI_Software_Timer(uint32_t *midi_counter_ptr, volatile uint32_t *statuses_ptr);

#endif /* SRC_MIDI_H_ */
