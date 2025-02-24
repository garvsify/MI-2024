//INCLUDES
#include "presets.h"

//VARIABLE DEFINITIONS
struct Preset presets_array[NUM_PRESETS] = {0};
struct Preset_Converted preset_converted_array[NUM_PRESETS] = {0};
enum Preset_Selected preset_selected = NO_PRESET_SELECTED;


//FUNCTION DEFINITIONS

uint8_t Update_Params_If_PC_Mode_Selected(void){

	if(waveshape_fsm.current_state == PC_MODE){
		Update_Waveshape_with_Converted_Preset_Value(&preset_converted_array[(uint8_t)preset_selected - 1], &params);
	}
	if(speed_fsm.current_state.shared_state == PC_MODE){
		Update_Speed_with_Converted_Preset_Value(&preset_converted_array[(uint8_t)preset_selected - 1], &params);
	}
	if(depth_fsm.current_state == PC_MODE){
		Update_Depth_with_Converted_Preset_Value(&preset_converted_array[(uint8_t)preset_selected - 1], &params);
	}
	if(symmetry_fsm.current_state == PC_MODE){
		Update_Symmetry_with_Converted_Preset_Value(&preset_converted_array[(uint8_t)preset_selected - 1], &params);
	}
	if(phase_fsm.current_state == PC_MODE){
		Update_Phase_with_Converted_Preset_Value(&preset_converted_array[(uint8_t)preset_selected - 1], &delay_line);
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


