#ifndef INC_CC_AND_PC_H_
#define INC_CC_AND_PC_H_

//MACROS
#define NUM_PRESETS 4

//INCLUDES
#include "midi_defines.h"
#include "oscillator.h"

//CUSTOM TYPES
struct Preset{

	volatile uint8_t waveshape;
	volatile uint8_t speed;
	volatile uint8_t depth;
	volatile uint8_t symmetry;
	volatile uint8_t phase;
};

enum Preset_Selected{

	NO_PRESET_SELECTED,
	PRESET_ONE,
	PRESET_TWO,
	PRESET_THREE,
	PRESET_FOUR
};

struct Preset_Converted{

	uint8_t waveshape; //7-bit
	uint16_t speed; //10-bit
	uint8_t depth; //7-bit
	uint16_t symmetry; //8-bit
	uint16_t phase; //9-bit
};

//VARIABLE DECLARATIONS
extern struct Preset factory_presets_array[4];
extern struct Preset presets_array[4];
extern enum Preset_Selected preset_selected;

//FUNCTION DECLARATIONS
uint8_t Update_Params_If_PC_Mode_Selected(void);
uint8_t Convert_All_Preset_Values(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr);
uint8_t Update_All_with_Converted_Preset_Values(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr, struct Delay_Line* delay_line_ptr);
uint8_t Update_Waveshape_with_Converted_Preset_Value(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr);
uint8_t Update_Speed_with_Converted_Preset_Value(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr);
uint8_t Update_Depth_with_Converted_Preset_Value(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr);
uint8_t Update_Symmetry_with_Converted_Preset_Value(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr);
uint8_t Update_Phase_with_Converted_Preset_Value(struct Preset_Converted* preset_converted_ptr, struct Delay_Line* delay_line_ptr);
uint8_t Convert_Waveshape_Preset_Value(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr);
uint8_t Convert_Speed_Preset_Value(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr);
uint8_t Convert_Depth_Preset_Value(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr);
uint8_t Convert_Symmetry_Preset_Value(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr);
uint8_t Convert_Phase_Preset_Value(struct Preset* preset_ptr, struct Preset_Converted* preset_converted_ptr);

#endif /* INC_CC_AND_PC_H_ */
