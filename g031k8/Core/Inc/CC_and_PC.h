#ifndef INC_CC_AND_PC_H_
#define INC_CC_AND_PC_H_

//MACROS
#define NUM_PRESETS 4

//pot CC values
#define WAVESHAPE_CC 20
#define SPEED_CC 21
#define DEPTH_CC 22
#define SYMMETRY_CC 23
#define PHASE_CC 24

#define WAVESHAPE_ARR 0
#define SPEED_ARR 1
#define DEPTH_ARR 2
#define SYMMETRY_ARR 3
#define PHASE_ARR 4

#define NUM_POTS 5

//FACTORY PRESET VALUES

//FACTORY PRESET 0 - square, reasonably fast, in phase, full depth
#define FACTORY_PRESET_0_WAVESHAPE 127
#define FACTORY_PRESET_0_SPEED 110
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

//TEST PRESET FOR TESTING FLASH MEMORY WRITE/READ
#define TEST_PRESET_3_WAVESHAPE 64
#define TEST_PRESET_3_SPEED 64
#define TEST_PRESET_3_DEPTH 64
#define TEST_PRESET_3_SYMMETRY 64
#define TEST_PRESET_3_PHASE 64


//USER PRESET FLASH MEMORY ADDRESSES
#define USER_PRESET_0_FLASH_MEMORY_ADDRESS 0x0800F800
//plus 8 bytes (64-bit)...
#define USER_PRESET_1_FLASH_MEMORY_ADDRESS 0x0800F808
//plus 8 bytes (64-bit)...
#define USER_PRESET_2_FLASH_MEMORY_ADDRESS 0x0800F810
//plus 8 bytes (64-bit)...
#define USER_PRESET_3_FLASH_MEMORY_ADDRESS 0x0800F818
//plus 8 bytes (64-bit)...
//64-bits used for whether user preset or factory preset for a given preset is used, start required before MIDI CLK status, MIDI basic channel N, and OMNI On/Off status
#define MISC_FLASH_MEMORY_ADDRESS 0x0800F820

//INCLUDES
#include "midi_defines.h"
#include "oscillator.h"
#include "MIDI.h"

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

	volatile uint8_t waveshape; //7-bit
	volatile uint16_t speed; //10-bit
	volatile uint8_t depth; //7-bit
	volatile uint16_t symmetry; //8-bit
	volatile uint16_t phase; //9-bit
};

//CONSTS
const extern struct Preset factory_preset_0;
const extern struct Preset factory_preset_1;
const extern struct Preset factory_preset_2;
const extern struct Preset factory_preset_3;

//TEST PRESET
const extern struct Preset test_preset;

//VARIABLE DECLARATIONS
extern const struct Preset *factory_presets_array[NUM_PRESETS];
volatile extern struct Preset *user_presets_array[NUM_PRESETS];
volatile extern struct Preset_Converted presets_converted_array[NUM_PRESETS];

volatile extern struct Preset user_preset_0;
volatile extern struct Preset user_preset_1;
volatile extern struct Preset user_preset_2;
volatile extern struct Preset user_preset_3;
volatile extern enum Validate user_presets_used_array[NUM_PRESETS];

volatile extern uint8_t CC_array[5];

volatile extern enum Preset_Selected preset_selected;

//FUNCTION DECLARATIONS
uint8_t Initialise_Preset_Arrays(void);
uint8_t Update_Params_Based_On_Mode_Selected(void);

uint8_t Convert_All_Preset_Values(volatile struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr);
uint8_t Update_All_with_Converted_Preset_Values(struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr);

uint8_t Update_Waveshape_with_Converted_Preset_Value(volatile struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr);
uint8_t Update_Speed_with_Converted_Preset_Value(volatile struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr);
uint8_t Update_Depth_with_Converted_Preset_Value(volatile struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr);
uint8_t Update_Symmetry_with_Converted_Preset_Value(volatile struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr);
uint8_t Update_Phase_with_Converted_Preset_Value(volatile struct Preset_Converted* preset_converted_ptr, struct Params* params_ptr);

uint8_t Convert_Waveshape_Preset_Value(struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr);
uint8_t Convert_Speed_Preset_Value(struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr);
uint8_t Convert_Depth_Preset_Value(struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr);
uint8_t Convert_Symmetry_Preset_Value(struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr);
uint8_t Convert_Phase_Preset_Value(struct Preset* preset_ptr, volatile struct Preset_Converted* preset_converted_ptr);

uint8_t Pack_Preset_Into_Doubleword(struct Preset* preset_ptr, uint64_t *Doubleword_ptr);
uint8_t Read_and_Interpret_Preset_From_Flash(uint32_t address_val, volatile struct Preset* preset_ptr);
uint8_t Read_and_Interpret_User_Presets_From_Flash(void);

uint8_t Pack_Misc_Into_Doubleword(enum Validate *user_presets_used_array_ptr, volatile uint32_t *statuses_ptr, volatile enum MIDI_Channel *MIDI_basic_channel_ptr, uint64_t *Doubleword_ptr, uint8_t num_presets);
uint8_t Read_and_Interpret_Misc_From_Flash(uint32_t address_val, volatile enum Validate *user_presets_used_array_ptr, volatile uint32_t *statuses_ptr, volatile enum MIDI_Channel *MIDI_basic_channel_ptr, uint8_t num_presets);

uint8_t Update_Converted_Preset_Array_with_User_or_Factory_Presets(volatile struct Preset_Converted* preset_converted_array_ptr,
																	volatile enum Validate *user_preset_used_array_ptr,
																	const struct Preset **factory_preset_array_ptr,
																	volatile struct Preset **user_preset_array_ptr,
																	uint8_t size_of_factory_and_user_arrays);
uint8_t Update_Waveshape_with_CC_Value(volatile uint8_t *data, struct Params* params_ptr);
uint8_t Update_Speed_with_CC_Value(volatile uint8_t *data, struct Params* params_ptr);
uint8_t Update_Depth_with_CC_Value(volatile uint8_t *data, struct Params* params_ptr);
uint8_t Update_Symmetry_with_CC_Value(volatile uint8_t *data, struct Params* params_ptr);
uint8_t Update_Phase_with_CC_Value(volatile uint8_t *data, struct Params* params_ptr);
uint8_t Set_Waveshape_to_CC_Mode_and_Value(uint8_t *data);
uint8_t Set_Speed_to_CC_Mode_and_Value(uint8_t *data);
uint8_t Set_Depth_to_CC_Mode_and_Value(uint8_t *data);
uint8_t Set_Symmetry_to_CC_Mode_and_Value(uint8_t *data);
uint8_t Set_Phase_to_CC_Mode_and_Value(uint8_t *data);
uint8_t Set_All_Pots_to_PC_Mode();

#endif /* INC_CC_AND_PC_H_ */
