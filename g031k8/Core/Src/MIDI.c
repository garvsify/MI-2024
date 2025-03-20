//INCLUDES
#include "MIDI.h"

//VARIABLE DEFINITIONS
volatile enum MIDI_Channel MIDI_basic_channel = MIDI_CH_ONE;
volatile uint8_t active_status_byte;
volatile uint8_t running_status_byte;
volatile struct MIDI_Data MIDI_data = {.MIDI_data_buffer = 0};

//FUNCTION DEFINITIONS
enum Validate Is_Status_Byte(volatile char *data){

	uint8_t MSB = (uint8_t) *data;

	MSB >>= 8;

	if(MSB){ //status byte

		return (enum Validate)YES;
	}
	else{ //data byte

		return (enum Validate)NO;
	}
}

enum Validate Is_PC_Status_Byte(volatile char *data){

	uint8_t source;
	uint8_t temp_data;

	source = (uint8_t)CHANNEL_VOICE_PROGRAM_CHANGE >> 4;
	temp_data = (uint8_t)*data;

	if(temp_data == source){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_CC_Status_Byte(volatile char *data){

	uint8_t source;
	uint8_t temp_data;

	source = (uint8_t)CHANNEL_VOICE_CONTROL_CHANGE >> 4;
	temp_data = (uint8_t)*data;

	if(temp_data == source){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_Sysex_Start_Status_Byte(volatile char *data){

	uint8_t temp_data = (uint8_t)*data;

	if(temp_data == (uint8_t)SYSTEM_EXCLUSIVE_START){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_Sysex_End_Status_Byte(volatile char *data){

	uint8_t temp_data = (uint8_t)*data;

	if(temp_data == (uint8_t)SYSTEM_EXCLUSIVE_END){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_Channel_Status_Byte_On_Basic_Channel(volatile char *data, volatile enum MIDI_Channel MIDI_basic_channel){

	uint8_t ch = (uint8_t)*data & 0x0F;
	uint8_t b_ch = (uint8_t)MIDI_basic_channel;

	if(ch == b_ch){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_Data_Buffer_Empty(struct MIDI_Data *MIDI_data_struct){

	uint16_t sum;

	for(uint8_t i = 0; i < sizeof(MIDI_data_struct->MIDI_data_buffer); i++){

		sum += MIDI_data_struct->MIDI_data_buffer[i];
	}

	if(sum == 0){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_Program_Change_Data_Byte_In_Range(volatile char *PC_data, uint8_t size_of_factory_or_user_array){

	uint8_t PCdata = (uint8_t) *PC_data;

	if(PCdata <= size_of_factory_or_user_array - 1){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_System_Real_Time_Status_Byte(volatile char *data){

	if(*data == SYSTEM_REAL_TIME_MIDI_CLOCK){
		return (enum Validate)YES;
	}
	else if(*data == SYSTEM_REAL_TIME_START){
		return (enum Validate)YES;
	}
	else if(*data == SYSTEM_REAL_TIME_CONTINUE){
		return (enum Validate)YES;
	}
	else if(*data == SYSTEM_REAL_TIME_STOP){
		return (enum Validate)YES;
	}
	else if(*data == SYSTEM_REAL_TIME_ACTIVE_SENSING){
		return (enum Validate)YES;
	}
	else if(*data == SYSTEM_REAL_TIME_RESET){
		return (enum Validate)YES;
	}
	else{
		return (enum Validate)NO;
	}
}

enum Validate Is_OMNI_On(volatile uint32_t *statuses_ptr){

	uint8_t omni_mode = Get_Status_Bit(statuses_ptr, MIDI_Channel_Voice_Mode);

	if(omni_mode == 1){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}
