//INCLUDES
#include <CC_and_PC.h>

//CONSTS DEFINITIONS
const struct Preset factory_preset_0 = {.waveshape = FACTORY_PRESET_0_WAVESHAPE, .speed = FACTORY_PRESET_0_SPEED, .depth = FACTORY_PRESET_0_DEPTH, .symmetry = FACTORY_PRESET_0_SYMMETRY, .phase = FACTORY_PRESET_0_PHASE};
const struct Preset factory_preset_1 = {.waveshape = FACTORY_PRESET_1_WAVESHAPE, .speed = FACTORY_PRESET_1_SPEED, .depth = FACTORY_PRESET_1_DEPTH, .symmetry = FACTORY_PRESET_1_SYMMETRY, .phase = FACTORY_PRESET_1_PHASE};
const struct Preset factory_preset_2 = {.waveshape = FACTORY_PRESET_2_WAVESHAPE, .speed = FACTORY_PRESET_2_SPEED, .depth = FACTORY_PRESET_2_DEPTH, .symmetry = FACTORY_PRESET_2_SYMMETRY, .phase = FACTORY_PRESET_2_PHASE};
const struct Preset factory_preset_3 = {.waveshape = FACTORY_PRESET_3_WAVESHAPE, .speed = FACTORY_PRESET_3_SPEED, .depth = FACTORY_PRESET_3_DEPTH, .symmetry = FACTORY_PRESET_3_SYMMETRY, .phase = FACTORY_PRESET_3_PHASE};

//VARIABLE DEFINITIONS
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

uint8_t Update_Params_If_PC_Mode_Selected(void){

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

uint8_t Read_Preset_From_Flash(uint32_t address_val, struct Preset* preset_ptr){

	uint8_t *flash = (uint8_t *)address_val;
	uint8_t *preset = (uint8_t *)preset_ptr;

	for(uint8_t i = 0; i < sizeof(*preset_ptr); i++){ //sizeof should work

		*(preset + i) = *(flash + i);

	}

	return 1;
}

uint8_t Pack_User_Preset_Used_Bytes_and_Start_Required_Before_MIDI_CLK_Into_Doubleword(enum Validate *user_presets_used_array_ptr, volatile uint32_t *statuses_ptr, uint64_t *Doubleword_ptr, uint8_t size_of_preset){

	uint64_t packed = 0;

	for(uint8_t i = 0; i < size_of_preset; i++){

		packed |= ((uint64_t)user_presets_used_array_ptr[i] << (i << 3)); //<< (i*8)
	}

	enum Validate start_required_before_midi_clk_status_bit = Get_Status_Bit(statuses_ptr, Start_Required_Before_Sync_Mode);

	packed |= (uint64_t)start_required_before_midi_clk_status_bit << (size_of_preset << 3); //<< (4 * 8)

	*Doubleword_ptr = packed;

	return 1;
}

uint8_t Read_and_Interpret_User_Preset_Used_Bytes_and_Start_Required_Before_MIDI_CLK_Byte_From_Flash(uint32_t address_val, enum Validate *user_presets_used_array_ptr, volatile uint32_t *statuses_ptr, uint8_t size_of_factory_or_user_array){

	uint8_t *address = (uint8_t *)address_val;

	uint8_t interpretted_value = 0;

	for(uint8_t i = 0; i < size_of_factory_or_user_array + 1; i++){

		interpretted_value = *(address + i);

		if(interpretted_value == (enum Validate)NO){

			if(i < size_of_factory_or_user_array){
				*(user_presets_used_array_ptr + i) = (enum Validate)NO;
			}
			else if(i == size_of_factory_or_user_array){
				Clear_Status_Bit(statuses_ptr, Start_Required_Before_Sync_Mode);
			}
		}
		else if((interpretted_value == 0xFF) || (interpretted_value == (enum Validate)YES)){

			if(i < size_of_factory_or_user_array){
				*(user_presets_used_array_ptr + i) = (enum Validate)YES;
			}
			else if(i == size_of_factory_or_user_array){
				Set_Status_Bit(statuses_ptr, Start_Required_Before_Sync_Mode);
			}
		}
	}

	return 1;
}

uint8_t Update_Converted_Preset_Array_with_User_or_Factory_Preset(struct Preset_Converted* presets_converted_array_ptr,
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

uint8_t Read_User_Presets_From_Flash(void){

	Read_Preset_From_Flash(USER_PRESET_0_FLASH_MEMORY_ADDRESS, &user_preset_0);
	Read_Preset_From_Flash(USER_PRESET_1_FLASH_MEMORY_ADDRESS, &user_preset_1);
	Read_Preset_From_Flash(USER_PRESET_2_FLASH_MEMORY_ADDRESS, &user_preset_2);
	Read_Preset_From_Flash(USER_PRESET_3_FLASH_MEMORY_ADDRESS, &user_preset_3);

	return 1;
}

