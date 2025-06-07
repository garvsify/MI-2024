//INCLUDES
#include <CC_and_PC.h>

//CONSTS DEFINITIONS
const struct Preset factory_preset_0 = {.waveshape = FACTORY_PRESET_0_WAVESHAPE, .speed = FACTORY_PRESET_0_SPEED, .depth = FACTORY_PRESET_0_DEPTH, .symmetry = FACTORY_PRESET_0_SYMMETRY, .phase = FACTORY_PRESET_0_PHASE};
const struct Preset factory_preset_1 = {.waveshape = FACTORY_PRESET_1_WAVESHAPE, .speed = FACTORY_PRESET_1_SPEED, .depth = FACTORY_PRESET_1_DEPTH, .symmetry = FACTORY_PRESET_1_SYMMETRY, .phase = FACTORY_PRESET_1_PHASE};
const struct Preset factory_preset_2 = {.waveshape = FACTORY_PRESET_2_WAVESHAPE, .speed = FACTORY_PRESET_2_SPEED, .depth = FACTORY_PRESET_2_DEPTH, .symmetry = FACTORY_PRESET_2_SYMMETRY, .phase = FACTORY_PRESET_2_PHASE};
const struct Preset factory_preset_3 = {.waveshape = FACTORY_PRESET_3_WAVESHAPE, .speed = FACTORY_PRESET_3_SPEED, .depth = FACTORY_PRESET_3_DEPTH, .symmetry = FACTORY_PRESET_3_SYMMETRY, .phase = FACTORY_PRESET_3_PHASE};

//VARIABLE DEFINITIONS
const struct Preset *factory_presets_array[NUM_PRESETS] = {0};
volatile struct Preset *user_presets_array[NUM_PRESETS] = {0};
volatile struct Preset_Converted presets_converted_array[NUM_PRESETS] = {0};
volatile enum Preset_Selected preset_selected = NO_PRESET_SELECTED;

//default values that may be selectively over-written by the user - and which are overwritten by flash-reads upon startup
volatile struct Preset user_preset_0 = factory_preset_0;
volatile struct Preset user_preset_1 = factory_preset_1;
volatile struct Preset user_preset_2 = factory_preset_2;
volatile struct Preset user_preset_3 = factory_preset_3;

volatile enum Validate user_presets_used_array[NUM_PRESETS] = {(enum Validate)NO};

volatile uint8_t CC_array[5] = {0};

//FUNCTION DEFINITIONS
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

	if(waveshape_fsm.current_state == MANUAL_MODE){
		params.waveshape = params_manual.waveshape;
	}
	if(speed_fsm.current_state.shared_state == MANUAL_MODE){
		params.speed = params_manual.speed;
	}
	if(depth_fsm.current_state == MANUAL_MODE){
		params.depth = params_manual.depth;
	}
	if(symmetry_fsm.current_state == MANUAL_MODE){
		params.symmetry = params_manual.symmetry;
	}
	if(phase_fsm.current_state == MANUAL_MODE){
		params.duty_delay_line_read_pointer_offset = params_manual.duty_delay_line_read_pointer_offset;
	}

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
		Update_Phase_with_Converted_Preset_Value(&presets_converted_array[(uint8_t)preset_selected - 1], &params);
	}


	if(waveshape_fsm.current_state == CC_MODE){
		Update_Waveshape_with_CC_Value(&CC_array[WAVESHAPE_ARR], &params);
	}
	if(speed_fsm.current_state.shared_state == CC_MODE){
		Update_Speed_with_CC_Value(&CC_array[SPEED_ARR], &params);
	}
	if(depth_fsm.current_state == CC_MODE){
		Update_Depth_with_CC_Value(&CC_array[DEPTH_ARR], &params);
	}
	if(symmetry_fsm.current_state == CC_MODE){
		Update_Symmetry_with_CC_Value(&CC_array[SYMMETRY_ARR], &params);
	}
	if(phase_fsm.current_state == CC_MODE){
		Update_Phase_with_CC_Value(&CC_array[PHASE_ARR], &params);
	}

	return 1;
}


uint8_t Convert_All_Preset_Values(volatile struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr){

	preset_converted_ptr->waveshape = preset_ptr->waveshape; //7-bit copy
	preset_converted_ptr->speed = (preset_ptr->speed) << 3; //convert to 10-bit
	preset_converted_ptr->depth = preset_ptr->depth; //7-bit copy
	preset_converted_ptr->symmetry = (preset_ptr->symmetry) << 1; //convert to 8-bit
	preset_converted_ptr->phase = (preset_ptr->phase) << 2; //convert to 9-bit

	return 1;
}

uint8_t Update_All_with_Converted_Preset_Values(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr){

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

	params_ptr->duty_delay_line_read_pointer_offset = preset_converted_ptr->phase;

	return 1;
}

uint8_t Update_Waveshape_with_Converted_Preset_Value(volatile struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr){

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

uint8_t Update_Speed_with_Converted_Preset_Value(volatile struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr){

	params_ptr->speed = preset_converted_ptr->speed;

	return 1;
}

uint8_t Update_Depth_with_Converted_Preset_Value(volatile struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr){

	params_ptr->depth = preset_converted_ptr->depth;

	return 1;
}

uint8_t Update_Symmetry_with_Converted_Preset_Value(volatile struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr){

	params_ptr->symmetry = preset_converted_ptr->symmetry;

	return 1;
}

uint8_t Update_Phase_with_Converted_Preset_Value(volatile struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr){

	params_ptr->duty_delay_line_read_pointer_offset = preset_converted_ptr->phase;

	return 1;
}

uint8_t Convert_Waveshape_Preset_Value(struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr){

	preset_converted_ptr->waveshape = preset_ptr->waveshape; //7-bit copy

	return 1;
}

uint8_t Convert_Speed_Preset_Value(struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr){

	preset_converted_ptr->speed = (preset_ptr->speed) << 3; //convert to 10-bit

	return 1;
}

uint8_t Convert_Depth_Preset_Value(struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr){

	preset_converted_ptr->depth = preset_ptr->depth; //7-bit copy

	return 1;
}

uint8_t Convert_Symmetry_Preset_Value(struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr){

	preset_converted_ptr->symmetry = (preset_ptr->symmetry) << 1; //convert to 8-bit

	return 1;
}

uint8_t Convert_Phase_Preset_Value(struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr){

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

uint8_t Read_and_Interpret_Preset_From_Flash(uint32_t address_val, volatile struct Preset* preset_ptr){

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

uint8_t Read_and_Interpret_Misc_From_Flash(uint32_t address_val, volatile enum Validate *user_presets_used_array_ptr, volatile uint32_t *statuses_ptr, volatile enum MIDI_Channel *MIDI_basic_channel_ptr, uint8_t num_presets){

	uint8_t *address = (uint8_t *)address_val;

	volatile uint8_t interpretted_value = 0;

	//PRESETS
	for(uint8_t i = 0; i < num_presets; i++){

		interpretted_value = *(address + i);

		if(interpretted_value == (enum Validate)YES){

			*(user_presets_used_array_ptr + i) = (enum Validate)YES;
		}
		else if((interpretted_value == 0xFF) || (interpretted_value == (enum Validate)NO)){

			*(user_presets_used_array_ptr + i) = (enum Validate)NO;
		}
	}

	//START REQUIRED BEFORE SYNC MODE
	interpretted_value = *(address + num_presets);

	if(interpretted_value == (enum Validate)NO){
		Clear_Status_Bit(statuses_ptr, Start_Required_Before_Sync_Mode);
	}
	else if((interpretted_value == (enum Validate)YES) || (interpretted_value == 0xFF)){
		Set_Status_Bit(statuses_ptr, Start_Required_Before_Sync_Mode);
	}

	//OMNI ON/OFF
	interpretted_value = *(address + num_presets + 1);

	if(interpretted_value == (enum Validate)YES){
		Set_Status_Bit(statuses_ptr, MIDI_Channel_Voice_Mode); //OMNI ON
	}
	else if((interpretted_value == (enum Validate)NO) || (interpretted_value == 0xFF)){
		Clear_Status_Bit(statuses_ptr, MIDI_Channel_Voice_Mode); //OMNI OFF
	}

	//BASIC CHANNEL
	interpretted_value = *(address + num_presets + 2);

	if(interpretted_value > (uint8_t)MIDI_CH_SIXTEEN){

		uint8_t midi_channel_default = MIDI_BASIC_CHANNEL_DEFAULT;

		*MIDI_basic_channel_ptr = (enum MIDI_Channel)midi_channel_default;
	}
	else{

		*MIDI_basic_channel_ptr = (enum MIDI_Channel)interpretted_value;
	}

	return 1;
}

uint8_t Update_Converted_Preset_Array_with_User_or_Factory_Presets(volatile struct Preset_Converted* presets_converted_array_ptr,
																	volatile enum Validate *user_presets_used_array_ptr,
																	const struct Preset **factory_presets_array_ptr,
																	volatile struct Preset **user_presets_array_ptr,
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

uint8_t Update_Waveshape_with_CC_Value(volatile uint8_t *data, struct Params* params_ptr){

	if(*data <= TRIANGLE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = TRIANGLE_MODE;
	}
	else if (*data <= SINE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = SINE_MODE;
	}
	else if (*data <= SQUARE_MODE_ADC_THRESHOLD){
		params_ptr->waveshape = SQUARE_MODE;
	}

	return 1;
}

uint8_t Update_Speed_with_CC_Value(volatile uint8_t *data, struct Params* params_ptr){

	uint16_t speed = (uint16_t)*data;

	speed <<= 3; //convert to 10-bit
	params_ptr->speed = speed;

	return 1;
}

uint8_t Update_Depth_with_CC_Value(volatile uint8_t *data, struct Params* params_ptr){

	uint8_t depth = (uint8_t)*data;
	params_ptr->depth = depth;

	return 1;
}

uint8_t Update_Symmetry_with_CC_Value(volatile uint8_t *data, struct Params* params_ptr){

	uint8_t symmetry = (uint8_t)*data;

	symmetry <<= 1; //convert to 8-bit
	params_ptr->symmetry = symmetry;

	return 1;
}

uint8_t Update_Phase_with_CC_Value(volatile uint8_t *data, struct Params* params_ptr){

	uint8_t phase = (uint8_t)*data;

	phase <<= 2; //convert to 9-bit
	params_ptr->duty_delay_line_read_pointer_offset = phase;

	return 1;
}

uint8_t Set_Waveshape_to_CC_Mode_and_Value(uint8_t *data){

	waveshape_fsm.prev_state = waveshape_fsm.current_state;
	waveshape_fsm.current_state = CC_MODE;

	CC_array[WAVESHAPE_ARR] = *data;

	return 1;
}

uint8_t Set_Speed_to_CC_Mode_and_Value(uint8_t *data){

	speed_fsm.prev_state.speed_exclusive_state = speed_fsm.current_state.speed_exclusive_state;
	speed_fsm.current_state.speed_exclusive_state = CC_MODE;

	CC_array[SPEED_ARR] = *data;

	return 1;
}

uint8_t Set_Depth_to_CC_Mode_and_Value(uint8_t *data){

	depth_fsm.prev_state = depth_fsm.current_state;
	depth_fsm.current_state = CC_MODE;

	CC_array[DEPTH_ARR] = *data;

	return 1;
}

uint8_t Set_Symmetry_to_CC_Mode_and_Value(uint8_t *data){

	symmetry_fsm.prev_state = symmetry_fsm.current_state;
	symmetry_fsm.current_state = CC_MODE;

	CC_array[SYMMETRY_ARR] = *data;

	return 1;
}

uint8_t Set_Phase_to_CC_Mode_and_Value(uint8_t *data){

	phase_fsm.prev_state = phase_fsm.current_state;
	phase_fsm.current_state = CC_MODE;

	CC_array[PHASE_ARR] = *data;

	return 1;
}

uint8_t Set_All_Pots_to_PC_Mode(void){

	waveshape_fsm.prev_state = waveshape_fsm.current_state;
	waveshape_fsm.current_state = PC_MODE;

	speed_fsm.prev_state.speed_exclusive_state = speed_fsm.current_state.speed_exclusive_state;
	speed_fsm.current_state.shared_state = PC_MODE;

	depth_fsm.prev_state = depth_fsm.current_state;
	depth_fsm.current_state = PC_MODE;

	symmetry_fsm.prev_state = symmetry_fsm.current_state;
	symmetry_fsm.current_state = PC_MODE;

	phase_fsm.prev_state = phase_fsm.current_state;
	phase_fsm.current_state = PC_MODE;

	return 1;
}

