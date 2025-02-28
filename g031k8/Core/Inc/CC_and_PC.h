#ifndef INC_CC_AND_PC_H_
#define INC_CC_AND_PC_H_

//MACROS
#define NUM_PRESETS 4


//FACTORY PRESET VALUES

//FACTORY PRESET 0 - square, reasonably fast, in phase, full depth
#define FACTORY_PRESET_0_WAVESHAPE 127
#define FACTORY_PRESET_0_SPEED 80
#define FACTORY_PRESET_0_DEPTH 127
#define FACTORY_PRESET_0_SYMMETRY 64
#define FACTORY_PRESET_0_PHASE 0

//FACTORY PRESET 1 - sine, reasonably slow, 180deg out-of-phase, half-depth
#define FACTORY_PRESET_1_WAVESHAPE 64
#define FACTORY_PRESET_1_SPEED 40
#define FACTORY_PRESET_1_DEPTH 64
#define FACTORY_PRESET_1_SYMMETRY 64
#define FACTORY_PRESET_1_PHASE 64

//FACTORY PRESET 2 - triangle, medium speed, full CCW symmetry, slightly out-of-phase
#define FACTORY_PRESET_1_WAVESHAPE 0
#define FACTORY_PRESET_1_SPEED 64
#define FACTORY_PRESET_1_DEPTH 127
#define FACTORY_PRESET_1_SYMMETRY 0
#define FACTORY_PRESET_1_PHASE 32

//FACTORY PRESET 3 - sine, medium speed, full CW symmetry, almost back-in-phase
#define FACTORY_PRESET_1_WAVESHAPE 64
#define FACTORY_PRESET_1_SPEED 64
#define FACTORY_PRESET_1_DEPTH 127
#define FACTORY_PRESET_1_SYMMETRY 127
#define FACTORY_PRESET_1_PHASE 100


//USER PRESET FLASH MEMORY ADDRESSES
#define USER_PRESET_0_FLASH_MEMORY_ADDRESS 0x0800F800
//plus 8 bytes (64-bit)...
#define USER_PRESET_1_FLASH_MEMORY_ADDRESS 0x0800F808
//plus 8 bytes (64-bit)...
#define USER_PRESET_2_FLASH_MEMORY_ADDRESS 0x0800F810
//plus 8 bytes (64-bit)...
#define USER_PRESET_3_FLASH_MEMORY_ADDRESS 0x0800F818
//plus 8 bytes (64-bit)...
//64-bits used for whether user preset or factory preset for a given preset are used, start required before midi clk bit, midi manufacturers ident, other midi system exclusive data, etc.
#define MISC_FLASH_MEMORY_ADDRESS 0x0800F820

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

//CONSTS
const extern struct Preset factory_preset_0;
const extern struct Preset factory_preset_1;
const extern struct Preset factory_preset_2;
const extern struct Preset factory_preset_3;

//VARIABLE DECLARATIONS
extern struct Preset *factory_presets_array[NUM_PRESETS];
extern struct Preset *presets_array[NUM_PRESETS];
extern struct Preset_Converted preset_converted_array[NUM_PRESETS] = {0};
extern enum Preset_Selected preset_selected;

extern struct Preset user_preset_0;
extern struct Preset user_preset_1;
extern struct Preset user_preset_2;
extern struct Preset user_preset_3;

extern enum Validate user_preset_0_used;
extern enum Validate user_preset_1_used;
extern enum Validate user_preset_2_used;
extern enum Validate user_preset_3_used;


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
uint8_t Pack_Preset_Into_Doubleword(struct Preset* preset_ptr, uint64_t *Doubleword_ptr);
uint8_t UnPack_Preset_From_Doubleword(uint64_t *Doubleword_ptr, struct Preset* preset_ptr);

#endif /* INC_CC_AND_PC_H_ */
