//INCLUDES
#include "MIDI.h"
#include "CC_and_PC.h"

//VARIABLE DEFINITIONS
volatile enum MIDI_Channel MIDI_basic_channel = MIDI_CH_ONE;
volatile uint8_t active_status_byte;
volatile uint8_t running_status_byte;
volatile struct MIDI_Data MIDI_data = {.MIDI_data_buffer = 0};

//FUNCTION DEFINITIONS
enum Validate Is_Status_Byte(volatile uint8_t *data){

	uint8_t MSB = *data;

	MSB >>= 7;

	if(MSB){ //status byte

		return (enum Validate)YES;
	}
	else{ //data byte

		return (enum Validate)NO;
	}
}

enum Validate Is_Data_Byte(volatile uint8_t *data){

	uint8_t MSB = *data;

	MSB >>= 7;

	if(MSB){ //status byte

		return (enum Validate)NO;
	}
	else{ //data byte

		return (enum Validate)YES;
	}
}

enum Validate Is_PC_Status_Byte(volatile uint8_t *data){

	uint8_t source;
	uint8_t temp_data;

	source = (uint8_t)CHANNEL_VOICE_PROGRAM_CHANGE >> 4;
	temp_data = *data >> 4;

	if(temp_data == source){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_CC_Status_Byte(volatile uint8_t *data){

	uint8_t source;
	uint8_t temp_data;

	source = (uint8_t)CHANNEL_VOICE_CONTROL_CHANGE >> 4;
	temp_data = *data >> 4;

	if(temp_data == source){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_Utilised_Channel_Mode_CC_First_Data_Byte(volatile uint8_t *first_data_byte){

	if(*first_data_byte >= CHANNEL_MODE_CC_THRESHOLD){

		if(*first_data_byte == RESET_ALL_CONTROLLERS){ //put all pots into CC mode and set to 50%

			return (enum Validate)YES;
		}
		else if(*first_data_byte == LOCAL_CONTROL){

			return (enum Validate)YES;
		}
		else if(*first_data_byte == OMNI_MODE_ON){

			return (enum Validate)YES;
		}
		else if(*first_data_byte == OMNI_MODE_OFF){

			return (enum Validate)YES;
		}
		else{

			return (enum Validate)NO;
		}
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_Utilised_CC_First_Data_Byte(volatile uint8_t *first_data_byte){

	if(*first_data_byte >= CHANNEL_MODE_CC_THRESHOLD){

		return (enum Validate)NO;
	}
	else{

		if(*first_data_byte == WAVESHAPE_CC){

			return (enum Validate)YES;
		}
		else if(*first_data_byte == SPEED_CC){

			return (enum Validate)YES;
		}
		else if(*first_data_byte == DEPTH_CC){

			return (enum Validate)YES;
		}
		else if(*first_data_byte == SYMMETRY_CC){

			return (enum Validate)YES;
		}
		else if(*first_data_byte == PHASE_CC){

			return (enum Validate)YES;
		}
		else{

			return (enum Validate)NO;
		}
	}
}

enum Validate Channel_Mode_CC_Second_Data_Byte_Is_Valid_Given_Utilised_First_Data_Byte(volatile uint8_t *first_data_byte, volatile uint8_t *second_data_byte){

	if(*first_data_byte == RESET_ALL_CONTROLLERS){

		if(*second_data_byte == 0){

			return (enum Validate)YES;
		}
		else{

			return (enum Validate)NO;
		}
	}
	else if(*first_data_byte == LOCAL_CONTROL){

		if((*second_data_byte == 0) || (*second_data_byte == 127)){

			return (enum Validate)YES;
		}
		else{

			return (enum Validate)NO;
		}
	}
	else if(*first_data_byte == OMNI_MODE_OFF){

		if((*second_data_byte == 0)){

			return (enum Validate)YES;
		}
		else{

			return (enum Validate)NO;
		}
	}
	else if(*first_data_byte == OMNI_MODE_ON){

		if((*second_data_byte == 0)){

			return (enum Validate)YES;
		}
		else{

			return (enum Validate)NO;
		}
	}
	else{

		return (enum Validate)NO; //should not even be called if first data byte is always valid
	}
}

uint8_t Process_Channel_Mode_Message(volatile struct MIDI_Data *MIDI_data_ptr, uint32_t *statuses_ptr){

	//once it has been determined a CC message on basic channel has been
	//received, and that the first and second data bytes are valid, this function enacts on the ch mode message

	if(MIDI_data_ptr->MIDI_data_buffer[0] == RESET_ALL_CONTROLLERS){

		//add code
	}
	else if(MIDI_data_ptr->MIDI_data_buffer[0] == LOCAL_CONTROL){

		if(MIDI_data_ptr->MIDI_data_buffer[1] == LOCAL_CONTROL_OFF){

			//add code
		}
		else if(MIDI_data_ptr->MIDI_data_buffer[1] == LOCAL_CONTROL_ON){

			//add code
		}
	}
	else if(MIDI_data_ptr->MIDI_data_buffer[0] == OMNI_MODE_OFF){

		Clear_Status_Bit(statuses_ptr, MIDI_Channel_Voice_Mode);
	}
	else if(MIDI_data_ptr->MIDI_data_buffer[0] == OMNI_MODE_ON){

		Set_Status_Bit(statuses_ptr, MIDI_Channel_Voice_Mode);
	}

	return 1;
}

enum Validate Is_Sysex_Start_Status_Byte(volatile uint8_t *data){

	if(*data == (uint8_t)SYSTEM_EXCLUSIVE_START){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_Sysex_End_Status_Byte(volatile uint8_t *data){

	if(*data == (uint8_t)SYSTEM_EXCLUSIVE_END){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_Channelised_Status_Byte_On_Basic_Channel(volatile uint8_t *data, volatile enum MIDI_Channel MIDI_basic_channel){

	uint8_t ch = *data & 0x0F;
	uint8_t b_ch = (uint8_t)MIDI_basic_channel;

	if(ch == b_ch){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_Data_Buffer_Empty(volatile struct MIDI_Data *MIDI_data_struct){

	uint16_t sum = 0;

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

uint8_t Num_Data_Bytes_Received(volatile struct MIDI_Data *MIDI_data_struct){

	if(Is_Data_Buffer_Empty(MIDI_data_struct) == YES){

		return 0;
	}
	else{

		/*if(MIDI_data_struct->MIDI_data_buffer[9] != 0){

			//will only be for Sysex end, and so we won't need to check how many bytes are present
		}*/
		if(MIDI_data_struct->MIDI_data_buffer[8] != 0){

			return 9;
		}
		else if(MIDI_data_struct->MIDI_data_buffer[7] != 0){

			return 8;
		}
		else if(MIDI_data_struct->MIDI_data_buffer[6] != 0){

			return 7;
		}
		else if(MIDI_data_struct->MIDI_data_buffer[5] != 0){

			return 6;
		}
		else if(MIDI_data_struct->MIDI_data_buffer[4] != 0){

			return 5;
		}
		else if(MIDI_data_struct->MIDI_data_buffer[3] != 0){

			return 4;
		}
		else if(MIDI_data_struct->MIDI_data_buffer[2] != 0){

			return 3;
		}
		else if(MIDI_data_struct->MIDI_data_buffer[1] != 0){

			return 2;
		}
		else{

			return 1;
		}
	}
}

uint8_t Clear_Data_Buffer(volatile struct MIDI_Data *MIDI_data_struct){

	for(uint8_t i = 0; i < sizeof(MIDI_data_struct->MIDI_data_buffer); i++){

		MIDI_data_struct->MIDI_data_buffer[i] = 0;
	}

	return 1;
}

enum Validate Is_Program_Change_Data_Byte_In_Range(volatile uint8_t *PC_data, uint8_t size_of_factory_or_user_array){

	if(*PC_data < size_of_factory_or_user_array){

		return (enum Validate)YES;
	}
	else{

		return (enum Validate)NO;
	}
}

enum Validate Is_System_Real_Time_Status_Byte(volatile uint8_t *data){

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

uint8_t Reset_and_Stop_MIDI_Software_Timer(uint32_t *midi_counter_ptr, volatile uint32_t *statuses_ptr){

	Clear_Status_Bit(statuses_ptr, Software_MIDI_Timer_Is_Running);
	Clear_Status_Bit(statuses_ptr, Software_MIDI_Timer_Has_Timed_Out);
	*midi_counter_ptr = 0;

	return 1;
}

uint8_t Reset_All_Controllers(struct Params *params_ptr, struct Delay_Line* delay_line_ptr){

	//Store previous states
	waveshape_fsm.prev_state = waveshape_fsm.current_state;
	speed_fsm.prev_state.speed_exclusive_state = speed_fsm.current_state.speed_exclusive_state;
	depth_fsm.prev_state = depth_fsm.current_state;
	symmetry_fsm.prev_state = symmetry_fsm.current_state;
	phase_fsm.prev_state = phase_fsm.current_state;

	//Put all pots into CC mode
	waveshape_fsm.current_state = CC_MODE;
	speed_fsm.current_state.speed_exclusive_state = CC_MODE;
	depth_fsm.current_state = CC_MODE;
	symmetry_fsm.current_state = CC_MODE;
	phase_fsm.current_state = CC_MODE;

	uint8_t data = 127 >> 1;

	for(uint8_t index = 0; index < NUM_POTS; index++){

		CC_array[index] = data;
	}

	return 1;
}

uint8_t Set_Local_Control(){

	//Store previous states
	waveshape_fsm.prev_state = waveshape_fsm.current_state;
	speed_fsm.prev_state.speed_exclusive_state = speed_fsm.current_state.speed_exclusive_state;
	depth_fsm.prev_state = depth_fsm.current_state;
	symmetry_fsm.prev_state = symmetry_fsm.current_state;
	phase_fsm.prev_state = phase_fsm.current_state;

	//Put all pots into manual mode
	waveshape_fsm.current_state = MANUAL_MODE;
	speed_fsm.current_state.speed_exclusive_state = MANUAL_MODE;
	depth_fsm.current_state = MANUAL_MODE;
	symmetry_fsm.current_state = MANUAL_MODE;
	phase_fsm.current_state = MANUAL_MODE;

	return 1;
}

uint8_t Set_OMNI_Off(volatile uint32_t *statuses_ptr){

	//Clear status bit
	Clear_Status_Bit(statuses_ptr, MIDI_Channel_Voice_Mode);

	return 1;
}

uint8_t Set_OMNI_On(volatile uint32_t *statuses_ptr){

	//Set status bit
	Set_Status_Bit(statuses_ptr, MIDI_Channel_Voice_Mode);

	return 1;
}
