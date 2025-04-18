//INCLUDES
#include "MIDI.h"

//VARIABLE DEFINITIONS
volatile enum MIDI_Channel MIDI_basic_channel = MIDI_CH_ONE;
volatile uint8_t active_status_byte;
volatile uint8_t running_status_byte;
volatile struct MIDI_Data MIDI_data = {.MIDI_data_buffer = 0};

const struct Preset factory_preset_0 = {.waveshape = FACTORY_PRESET_0_WAVESHAPE, .speed = FACTORY_PRESET_0_SPEED, .depth = FACTORY_PRESET_0_DEPTH, .symmetry = FACTORY_PRESET_0_SYMMETRY, .phase = FACTORY_PRESET_0_PHASE};
const struct Preset factory_preset_1 = {.waveshape = FACTORY_PRESET_1_WAVESHAPE, .speed = FACTORY_PRESET_1_SPEED, .depth = FACTORY_PRESET_1_DEPTH, .symmetry = FACTORY_PRESET_1_SYMMETRY, .phase = FACTORY_PRESET_1_PHASE};
const struct Preset factory_preset_2 = {.waveshape = FACTORY_PRESET_2_WAVESHAPE, .speed = FACTORY_PRESET_2_SPEED, .depth = FACTORY_PRESET_2_DEPTH, .symmetry = FACTORY_PRESET_2_SYMMETRY, .phase = FACTORY_PRESET_2_PHASE};
const struct Preset factory_preset_3 = {.waveshape = FACTORY_PRESET_3_WAVESHAPE, .speed = FACTORY_PRESET_3_SPEED, .depth = FACTORY_PRESET_3_DEPTH, .symmetry = FACTORY_PRESET_3_SYMMETRY, .phase = FACTORY_PRESET_3_PHASE};

const struct Preset *factory_presets_array[NUM_PRESETS] = {0};
struct Preset *user_presets_array[NUM_PRESETS] = {0};
struct Preset_Converted presets_converted_array[NUM_PRESETS] = {0};
enum Preset_Selected preset_selected = NO_PRESET_SELECTED;

//default values that may be selectively over-written by the user - and which are overwritten by flash-reads upon startup
struct Preset user_preset_0 = factory_preset_0;
struct Preset user_preset_1 = factory_preset_1;
struct Preset user_preset_2 = factory_preset_2;
struct Preset user_preset_3 = factory_preset_3;

enum Validate user_presets_used_array[NUM_PRESETS] = {(enum Validate)NO};

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

enum Validate Is_Data_Byte(volatile char *data){

	uint8_t MSB = (uint8_t) *data;

	MSB >>= 8;

	if(MSB){ //status byte

		return (enum Validate)NO;
	}
	else{ //data byte

		return (enum Validate)YES;
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

enum Validate Is_Utilised_Channel_Mode_CC_First_Data_Byte(volatile char *first_data_byte){

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

enum Validate Is_Utilised_CC_First_Data_Byte(volatile char *first_data_byte){

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

enum Validate Channel_Mode_CC_Second_Data_Byte_Is_Valid_Given_Utilised_First_Data_Byte(volatile char *first_data_byte, volatile char *second_data_byte){

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

uint8_t Process_Channel_Mode_Message(volatile struct MIDI_Data *MIDI_data_ptr, volatile uint32_t *statuses_ptr){

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

uint8_t Process_CC_Message(volatile struct MIDI_Data *MIDI_data_ptr, struct Params *params_ptr, struct Delay_Line* delay_line_ptr){

	//once it has been determined a CC message on basic channel/when OMNI enabled has been
	//received, and that the first data byte is valid, this function enacts on the CC message

	if(MIDI_data_ptr->MIDI_data_buffer[0] == WAVESHAPE_CC){

		Update_Waveshape_with_CC_Value(&MIDI_data_ptr->MIDI_data_buffer[1], params_ptr);
		waveshape_fsm.prev_state = waveshape_fsm.current_state;
		waveshape_fsm.current_state = CC_MODE;
	}
	else if(MIDI_data_ptr->MIDI_data_buffer[0] == SPEED_CC){

		Update_Speed_with_CC_Value(&MIDI_data_ptr->MIDI_data_buffer[1], params_ptr);
		speed_fsm.prev_state.shared_state = speed_fsm.current_state.shared_state;
		speed_fsm.current_state.shared_state = CC_MODE;
	}
	else if(MIDI_data_ptr->MIDI_data_buffer[0] == DEPTH_CC){

		Update_Depth_with_CC_Value(&MIDI_data_ptr->MIDI_data_buffer[1], params_ptr);
		depth_fsm.prev_state = depth_fsm.current_state;
		depth_fsm.current_state = CC_MODE;
	}
	else if(MIDI_data_ptr->MIDI_data_buffer[0] == SYMMETRY_CC){

		Update_Symmetry_with_CC_Value(&MIDI_data_ptr->MIDI_data_buffer[1], params_ptr);
		symmetry_fsm.prev_state = symmetry_fsm.current_state;
		symmetry_fsm.current_state = CC_MODE;
	}
	else if(MIDI_data_ptr->MIDI_data_buffer[0] == PHASE_CC){

		Update_Phase_with_CC_Value(&MIDI_data_ptr->MIDI_data_buffer[1], delay_line_ptr);
		phase_fsm.prev_state = phase_fsm.current_state;
		phase_fsm.current_state = CC_MODE;
	}

	return 1;
}

uint8_t Reset_All_Controllers(struct Params *params_ptr){

	//Put all pots into CC mode

	//Set the value of the pots to 50%

	return 1;
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

uint8_t Reset_and_Stop_MIDI_Software_Timer(uint32_t *midi_counter_ptr, volatile uint32_t *statuses_ptr){

	Clear_Status_Bit(statuses_ptr, Software_MIDI_Timer_Is_Running);
	Clear_Status_Bit(statuses_ptr, Software_MIDI_Timer_Has_Timed_Out);
	*midi_counter_ptr = 0;

	return 1;
}

uint8_t Initialise_Preset_Arrays(void){

	for(uint8_t i = 0; i < sizeof(factory_presets_array); i++){

		if(i == 0){
			factory_presets_array[i] = &factory_preset_0;
			user_presets_array[i] = &user_preset_0;
		}
		else if(i == 1){
			factory_presets_array[i] = &factory_preset_1;
			user_presets_array[i] = &user_preset_1;
		}
		else if(i == 2){
			factory_presets_array[i] = &factory_preset_2;
			user_presets_array[i] = &user_preset_2;
		}
		else if(i == 3){
			factory_presets_array[i] = &factory_preset_3;
			user_presets_array[i] = &user_preset_3;
		}
	}

	return 1;
}

uint8_t Update_Params_Based_On_Mode_Selected(void){

	if(waveshape_fsm.current_state == PC_MODE){
		Update_Waveshape_with_Converted_Preset_Value(&presets_converted_array[(uint8_t)preset_selected - 1], &params);
	}
	if(speed_fsm.current_state.shared_state == PC_MODE){
		Update_Speed_with_Converted_Preset_Value(&presets_converted_array[(uint8_t)preset_selected - 1], &params);
	}
	if(depth_fsm.current_state == PC_MODE){
		Update_Depth_with_Converted_Preset_Value(&presets_converted_array[(uint8_t)preset_selected - 1], &params);
	}
	if(symmetry_fsm.current_state == PC_MODE){
		Update_Symmetry_with_Converted_Preset_Value(&presets_converted_array[(uint8_t)preset_selected - 1], &params);
	}
	if(phase_fsm.current_state == PC_MODE){
		Update_Phase_with_Converted_Preset_Value(&presets_converted_array[(uint8_t)preset_selected - 1], &delay_line);
	}



	if(waveshape_fsm.current_state == CC_MODE){

	}
	if(speed_fsm.current_state.shared_state == CC_MODE){

	}
	if(depth_fsm.current_state == CC_MODE){

	}
	if(symmetry_fsm.current_state == CC_MODE){

	}
	if(phase_fsm.current_state == CC_MODE){

	}

	return 1;
}


uint8_t Convert_All_Preset_Values(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr){

	preset_converted_ptr->waveshape = preset_ptr->waveshape; //7-bit copy
	preset_converted_ptr->speed = (preset_ptr->speed) << 3; //convert to 10-bit
	preset_converted_ptr->depth = preset_ptr->depth; //7-bit copy
	preset_converted_ptr->symmetry = (preset_ptr->symmetry) << 1; //convert to 8-bit
	preset_converted_ptr->phase = (preset_ptr->phase) << 2; //convert to 9-bit

	return 1;
}

uint8_t Update_All_with_Converted_Preset_Values(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr, struct Delay_Line* delay_line_ptr){

	uint8_t waveshape = preset_converted_ptr->waveshape;

	if(waveshape <= TRIANGLE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = TRIANGLE_MODE; //triangle wave
	}
	else if (waveshape <= SINE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = SINE_MODE; //sine wave
	}
	else if (waveshape <= SQUARE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = SQUARE_MODE; //square wave
	}

	params_ptr->speed = preset_converted_ptr->speed;

	params_ptr->depth = preset_converted_ptr->depth;

	params_ptr->symmetry = preset_converted_ptr->symmetry;

	delay_line_ptr->duty_delay_line_read_pointer_offset = preset_converted_ptr->phase;

	return 1;
}

uint8_t Update_Waveshape_with_Converted_Preset_Value(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr){

	uint8_t waveshape = preset_converted_ptr->waveshape;

	if(waveshape <= TRIANGLE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = TRIANGLE_MODE; //triangle wave
	}
	else if (waveshape <= SINE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = SINE_MODE; //sine wave
	}
	else if (waveshape <= SQUARE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = SQUARE_MODE; //square wave
	}

	return 1;
}

uint8_t Update_Speed_with_Converted_Preset_Value(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr){

	params_ptr->speed = preset_converted_ptr->speed;

	return 1;
}

uint8_t Update_Depth_with_Converted_Preset_Value(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr){

	params_ptr->depth = preset_converted_ptr->depth;

	return 1;
}

uint8_t Update_Symmetry_with_Converted_Preset_Value(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr){

	params_ptr->symmetry = preset_converted_ptr->symmetry;

	return 1;
}

uint8_t Update_Phase_with_Converted_Preset_Value(struct Preset_Converted* preset_converted_ptr, struct Delay_Line* delay_line_ptr){

	delay_line_ptr->duty_delay_line_read_pointer_offset = preset_converted_ptr->phase;

	return 1;
}

uint8_t Convert_Waveshape_Preset_Value(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr){

	preset_converted_ptr->waveshape = preset_ptr->waveshape; //7-bit copy

	return 1;
}

uint8_t Convert_Speed_Preset_Value(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr){

	preset_converted_ptr->speed = (preset_ptr->speed) << 3; //convert to 10-bit

	return 1;
}

uint8_t Convert_Depth_Preset_Value(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr){

	preset_converted_ptr->depth = preset_ptr->depth; //7-bit copy

	return 1;
}

uint8_t Convert_Symmetry_Preset_Value(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr){

	preset_converted_ptr->symmetry = (preset_ptr->symmetry) << 1; //convert to 8-bit

	return 1;
}

uint8_t Convert_Phase_Preset_Value(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr){

	preset_converted_ptr->phase = (preset_ptr->phase) << 2; //convert to 9-bit

	return 1;
}

uint8_t Pack_Preset_Into_Doubleword(struct Preset* preset_ptr, uint64_t *Doubleword_ptr){

	uint64_t packed = 0;

	for(uint8_t i = 0; i < sizeof(*preset_ptr); i++){

		packed |= ((uint64_t)*((uint8_t *)preset_ptr) << (i << 3)); //<< (i*8)
	}

	*Doubleword_ptr = packed;

	return 1;
}

uint8_t Read_and_Interpret_Preset_From_Flash(uint32_t address_val, struct Preset* preset_ptr){

	uint8_t *flash = (uint8_t *)address_val;
	uint8_t *preset = (uint8_t *)preset_ptr;

	for(uint8_t i = 0; i < sizeof(*preset_ptr); i++){ //sizeof should work

		if(*(flash + i) > 0x7F){ //127
			*(preset + i) = 0x7F;
		}
		else{
			*(preset + i) = *(flash + i);
		}
	}

	return 1;
}

uint8_t Pack_Misc_Into_Doubleword(enum Validate *user_presets_used_array_ptr, volatile uint32_t *statuses_ptr, volatile enum MIDI_Channel *MIDI_basic_channel_ptr, uint64_t *Doubleword_ptr, uint8_t num_presets){

	//This function will break if presets are bigger than 5

	uint64_t packed = 0;

	//USER PRESETS USED ARRAY
	for(uint8_t i = 0; i < num_presets; i++){

		packed |= ((uint64_t)user_presets_used_array_ptr[i] << (i << 3)); //<< (i*8)
	}

	//START REQUIRED BEFORE MIDI CLK
	enum Validate start_required_before_midi_clk_status_bit = Get_Status_Bit(statuses_ptr, Start_Required_Before_Sync_Mode);

	uint8_t shift = num_presets << 3; //<< (4 * 8)
	packed |= (uint64_t)start_required_before_midi_clk_status_bit << shift;

	//OMNI ON/OFF (MIDI CHANNEL VOICE MODE)
	enum Validate MIDI_channel_voice_mode_status_bit = Get_Status_Bit(statuses_ptr, MIDI_Channel_Voice_Mode);

	packed |= (uint64_t)MIDI_channel_voice_mode_status_bit << (shift + (1 << 3)); //(8 * 5)

	//MIDI BASIC CHANNEL
	packed |= (uint64_t) *MIDI_basic_channel_ptr << (shift + (2 << 3));

	*Doubleword_ptr = packed;

	return 1;
}

uint8_t Read_and_Interpret_Misc_From_Flash(uint32_t address_val, enum Validate *user_presets_used_array_ptr, volatile uint32_t *statuses_ptr, volatile enum MIDI_Channel *MIDI_basic_channel_ptr, uint8_t num_presets){

	uint8_t *address = (uint8_t *)address_val;

	uint8_t interpretted_value = 0;

	//PRESETS
	for(uint8_t i = 0; i < num_presets; i++){

		interpretted_value = *(address + i);

		if(interpretted_value == (enum Validate)NO){

			*(user_presets_used_array_ptr + i) = (enum Validate)NO;
		}
		else if((interpretted_value == 0xFF) || (interpretted_value == (enum Validate)YES)){

			*(user_presets_used_array_ptr + i) = (enum Validate)YES;
		}
	}

	//START REQUIRED BEFORE SYNC MODE
	interpretted_value = *(address + num_presets);

	if(interpretted_value == (enum Validate)NO){
		Clear_Status_Bit(statuses_ptr, Start_Required_Before_Sync_Mode);
	}
	else if(interpretted_value == (enum Validate)YES){
		Set_Status_Bit(statuses_ptr, Start_Required_Before_Sync_Mode);
	}

	//OMNI ON/OFF
	interpretted_value = *(address + num_presets + 1);

	if(interpretted_value == (enum Validate)NO){
		Clear_Status_Bit(statuses_ptr, MIDI_Channel_Voice_Mode); //OMNI OFF
	}
	else if(interpretted_value == (enum Validate)YES){
		Set_Status_Bit(statuses_ptr, MIDI_Channel_Voice_Mode); //OMNI ON
	}

	//BASIC CHANNEL
	interpretted_value = *(address + num_presets + 2);

	if(interpretted_value >= 16){

		uint8_t midi_channel_default = MIDI_BASIC_CHANNEL_DEFAULT;

		*MIDI_basic_channel_ptr = (enum MIDI_Channel)midi_channel_default;
	}
	else{

		*MIDI_basic_channel_ptr = (enum MIDI_Channel)interpretted_value;
	}

	return 1;
}

uint8_t Update_Converted_Preset_Array_with_User_or_Factory_Presets(struct Preset_Converted* presets_converted_array_ptr,
																	enum Validate *user_presets_used_array_ptr,
																	const struct Preset **factory_presets_array_ptr,
																	struct Preset **user_presets_array_ptr,
																	uint8_t size_of_factory_and_user_arrays){

	for(uint8_t i = 0; i < size_of_factory_and_user_arrays; i++){

		if(*(user_presets_used_array_ptr + i) == (enum Validate)YES){
			Convert_All_Preset_Values(*(user_presets_array_ptr + i), (presets_converted_array_ptr + i));
		}
		else if(*(user_presets_used_array_ptr + i) == (enum Validate)NO){
			Convert_All_Preset_Values((struct Preset *) *(factory_presets_array_ptr + i), (presets_converted_array_ptr + i));
		}
	}

	return 1;
}

uint8_t Read_and_Interpret_User_Presets_From_Flash(void){

	Read_and_Interpret_Preset_From_Flash(USER_PRESET_0_FLASH_MEMORY_ADDRESS, &user_preset_0);
	Read_and_Interpret_Preset_From_Flash(USER_PRESET_1_FLASH_MEMORY_ADDRESS, &user_preset_1);
	Read_and_Interpret_Preset_From_Flash(USER_PRESET_2_FLASH_MEMORY_ADDRESS, &user_preset_2);
	Read_and_Interpret_Preset_From_Flash(USER_PRESET_3_FLASH_MEMORY_ADDRESS, &user_preset_3);

	return 1;
}

uint8_t Update_Waveshape_with_CC_Value(volatile char *data, struct Params* params_ptr){

	uint8_t waveshape = (uint8_t)*data;

	if(waveshape <= TRIANGLE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = TRIANGLE_MODE;
	}
	else if (waveshape <= SINE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = SINE_MODE;
	}
	else if (waveshape <= SQUARE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = SQUARE_MODE;
	}

	return 1;
}

uint8_t Update_Speed_with_CC_Value(volatile char *data, struct Params* params_ptr){

	uint16_t speed = (uint16_t)*data;

	speed <<= 3; //convert to 10-bit
	params_ptr->speed = speed;

	return 1;
}

uint8_t Update_Depth_with_CC_Value(volatile char *data, struct Params* params_ptr){

	uint8_t depth = (uint8_t)*data;
	params_ptr->depth = depth;

	return 1;
}

uint8_t Update_Symmetry_with_CC_Value(volatile char *data, struct Params* params_ptr){

	uint8_t symmetry = (uint8_t)*data;

	symmetry <<= 1; //convert to 8-bit
	params_ptr->symmetry = symmetry;

	return 1;
}

uint8_t Update_Phase_with_CC_Value(volatile char *data, struct Delay_Line* delay_line_ptr){

	uint8_t phase = (uint8_t)*data;

	phase <<= 2; //convert to 9-bit
	delay_line_ptr->duty_delay_line_read_pointer_offset = phase;

	return 1;
}
