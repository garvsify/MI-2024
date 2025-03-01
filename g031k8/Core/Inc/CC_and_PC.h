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
#define FACTORY_PRESET_2_WAVESHAPE 0
#define FACTORY_PRESET_2_SPEED 64
#define FACTORY_PRESET_2_DEPTH 127
#define FACTORY_PRESET_2_SYMMETRY 0
#define FACTORY_PRESET_2_PHASE 32

//FACTORY PRESET 3 - sine, medium speed, full CW symmetry, almost back-in-phase
#define FACTORY_PRESET_3_WAVESHAPE 64
#define FACTORY_PRESET_3_SPEED 64
#define FACTORY_PRESET_3_DEPTH 127
#define FACTORY_PRESET_3_SYMMETRY 127
#define FACTORY_PRESET_3_PHASE 100


//USER PRESET FLASH MEMORY ADDRESSES
#define USER_PRESET_0_FLASH_MEMORY_ADDRESS 0x0800F800
//plus 8 bytes (64-bit)...
#define USER_PRESET_1_FLASH_MEMORY_ADDRESS 0x0800F808
//plus 8 bytes (64-bit)...
#define USER_PRESET_2_FLASH_MEMORY_ADDRESS 0x0800F810
//plus 8 bytes (64-bit)...
#define USER_PRESET_3_FLASH_MEMORY_ADDRESS 0x0800F818
//plus 8 bytes (64-bit)...
//64-bits used for whether user preset or factory preset for a given preset is used, start required before midi clk bool
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

struct User_Preset_X_Used{

	enum Validate user_preset_0_used;
	enum Validate user_preset_1_used;
	enum Validate user_preset_2_used;
	enum Validate user_preset_3_used;
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
extern struct Preset_Converted preset_converted_array[NUM_PRESETS];
extern enum Preset_Selected preset_selected;

extern struct Preset user_preset_0;
extern struct Preset user_preset_1;
extern struct Preset user_preset_2;
extern struct Preset user_preset_3;

extern struct User_Preset_X_Used user_preset_x_used_struct;


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
uint8_t Read_Preset_From_Flash(uint32_t address, struct Preset* preset_ptr);
uint8_t Pack_User_Preset_Used_Bytes_and_Start_Required_Before_MIDI_CLK_Into_Doubleword(struct User_Preset_X_Used *user_preset_used_struct_ptr, enum Status_Bit *start_required_before_midi_clk_status_bit_ptr, uint64_t *Doubleword_ptr);
uint8_t Read_User_Preset_Used_Bytes_and_Start_Required_Before_MIDI_CLK_Byte_From_Flash(uint32_t address_val, struct User_Preset_X_Used *user_preset_used_struct_ptr, enum Status_Bit *start_required_before_midi_clk_status_bit_ptr);


#endif /* INC_CC_AND_PC_H_ */
