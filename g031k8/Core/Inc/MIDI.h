#ifndef SRC_MIDI_H_
#define SRC_MIDI_H_

#define NUM_PRESETS 4

//pot CC values
#define WAVESHAPE_CC 20
#define SPEED_CC 21
#define DEPTH_CC 22
#define SYMMETRY_CC 23
#define PHASE_CC 24


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
//64-bits used for whether user preset or factory preset for a given preset is used, start required before MIDI CLK status, MIDI basic channel N, and OMNI On/Off status
#define MISC_FLASH_MEMORY_ADDRESS 0x0800F820


#define MIDI_BASIC_CHANNEL_DEFAULT 0 //CH1

#define CHANNEL_MODE_CC_THRESHOLD 120

//channel mode first data byte defines
//#define ALL_SOUND_OFF 120
#define RESET_ALL_CONTROLLERS 121
#define LOCAL_CONTROL 122
//#define ALL_NOTES_OFF 123
#define OMNI_MODE_OFF 124
#define OMNI_MODE_ON 125
//#define MONO_MODE_ON 126
//#define POLY_MODE_ON 127

#define LOCAL_CONTROL_OFF 0
#define LOCAL_CONTROL_ON 127


//INCLUDES
#include <stdint.h>
#include "utility.h"
#include "symmetry_adjustment.h"
#include "oscillator_params_type.h"
#include "config.h"
#include "timers.h"
#include "oscillator_macros.h"
#include "midi_defines.h"
#include "oscillator.h"
#include "delay_line.h"


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

enum MIDI_Channel{
	MIDI_CH_ONE,
	MIDI_CH_TWO,
	MIDI_CH_THREE,
	MIDI_CH_FOUR,
	MIDI_CH_FIVE,
	MIDI_CH_SIX,
	MIDI_CH_SEVEN,
	MIDI_CH_EIGHT,
	MIDI_CH_NINE,
	MIDI_CH_TEN,
	MIDI_CH_ELEVEN,
	MIDI_CH_TWELVE,
	MIDI_CH_THIRTEEN,
	MIDI_CH_FOURTEEN,
	MIDI_CH_FIFTEEN,
	MIDI_CH_SIXTEEN,
};

struct MIDI_Data{

	volatile char MIDI_data_buffer[10];
};

//CONSTS
const extern struct Preset factory_preset_0;
const extern struct Preset factory_preset_1;
const extern struct Preset factory_preset_2;
const extern struct Preset factory_preset_3;

//VARIABLE DECLARATIONS

extern enum Validate user_presets_used_array[NUM_PRESETS];

extern const struct Preset *factory_presets_array[NUM_PRESETS];
extern struct Preset *user_presets_array[NUM_PRESETS];
extern struct Preset_Converted presets_converted_array[NUM_PRESETS];
extern enum Preset_Selected preset_selected;

extern struct Preset user_preset_0;
extern struct Preset user_preset_1;
extern struct Preset user_preset_2;
extern struct Preset user_preset_3;

volatile extern enum MIDI_Channel MIDI_basic_channel;
volatile extern uint8_t active_status_byte;
volatile extern uint8_t running_status_byte;
volatile extern struct MIDI_Data MIDI_data;


//FUNCTION DECLARATIONS
enum Validate Is_Status_Byte(volatile char *data);
enum Validate Is_Data_Byte(volatile char *data);
enum Validate Is_PC_Status_Byte(volatile char *data);
enum Validate Is_CC_Status_Byte(volatile char *data);
enum Validate Is_Utilised_Channel_Mode_CC_First_Data_Byte(volatile char *first_data_byte);
enum Validate Is_Utilised_CC_First_Data_Byte(volatile char *first_data_byte);
enum Validate Channel_Mode_CC_Second_Data_Byte_Is_Valid_Given_Utilised_First_Data_Byte(volatile char *first_data_byte, volatile char *second_data_byte);
uint8_t Process_CC_Message(volatile struct MIDI_Data *MIDI_data_ptr, struct Params *params_ptr, struct Delay_Line *delay_line_ptr);
uint8_t Process_Channel_Mode_Message(volatile struct MIDI_Data *MIDI_data_ptr, volatile uint32_t *statuses_ptr);
enum Validate Is_Sysex_Start_Status_Byte(volatile char *data);
enum Validate Is_Sysex_End_Status_Byte(volatile char *data);
enum Validate Is_Channel_Status_Byte_On_Basic_Channel(volatile char *data, volatile enum MIDI_Channel MIDI_basic_channel);
enum Validate Is_Data_Buffer_Empty(volatile struct MIDI_Data *MIDI_data_struct);
uint8_t Num_Data_Bytes_Received(volatile struct MIDI_Data *MIDI_data_struct);
uint8_t Clear_Data_Buffer(volatile struct MIDI_Data *MIDI_data_struct);
enum Validate Is_Program_Change_Data_Byte_In_Range(volatile char *PC_data, uint8_t size_of_factory_or_user_array);
enum Validate Is_System_Real_Time_Status_Byte(volatile char *data);
enum Validate Is_OMNI_On(volatile uint32_t *statuses_ptr);
uint8_t Reset_and_Stop_MIDI_Software_Timer(uint32_t *midi_counter_ptr, volatile uint32_t *statuses_ptr);
uint8_t Initialise_Preset_Arrays(void);
uint8_t Update_Params_Based_On_Mode_Selected(void);

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
uint8_t Read_and_Interpret_Preset_From_Flash(uint32_t address_val, struct Preset* preset_ptr);
uint8_t Read_and_Interpret_User_Presets_From_Flash(void);

uint8_t Pack_Misc_Into_Doubleword(enum Validate *user_presets_used_array_ptr, volatile uint32_t *statuses_ptr, volatile enum MIDI_Channel *MIDI_basic_channel_ptr, uint64_t *Doubleword_ptr, uint8_t num_presets);
uint8_t Read_and_Interpret_Misc_From_Flash(uint32_t address_val, enum Validate *user_presets_used_array_ptr, volatile uint32_t *statuses_ptr, volatile enum MIDI_Channel *MIDI_basic_channel_ptr, uint8_t num_presets);

uint8_t Update_Converted_Preset_Array_with_User_or_Factory_Presets(struct Preset_Converted* preset_converted_array_ptr,
																	enum Validate *user_preset_used_array_ptr,
																	const struct Preset **factory_preset_array_ptr,
																	struct Preset **user_preset_array_ptr,
																	uint8_t size_of_factory_and_user_arrays);

uint8_t Update_Waveshape_with_CC_Value(volatile char *data, struct Params* params_ptr);
uint8_t Update_Speed_with_CC_Value(volatile char *data, struct Params* params_ptr);
uint8_t Update_Depth_with_CC_Value(volatile char *data, struct Params* params_ptr);
uint8_t Update_Symmetry_with_CC_Value(volatile char *data, struct Params* params_ptr);
uint8_t Update_Phase_with_CC_Value(volatile char *data, struct Delay_Line* delay_line_ptr);

#endif /* SRC_MIDI_H_ */
