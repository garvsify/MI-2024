#ifndef MIDI_DEFINES_H
#define MIDI_DEFINES_H

#include <stdint.h>

#define SOFTWARE_MIDI_TIMER_IS_RUNNING (0b1 << 8)
#define SOFTWARE_MIDI_TIMER_HAS_TIMED_OUT (0b1 << 9)
#define MIDI_CHANNEL_VOICE_MODE (0b1 << 10)

#define MIDI_MS_NIBBLE_SHIFT 4

volatile extern uint8_t MIDI_CLK_tag;

enum MIDI_Status_Byte{

	//CHANNEL VOICE
	//CHANNEL_VOICE_NOTE_OFF = 0x80, //second nibble is channel
	//CHANNEL_VOICE_NOTE_ON = 0x90, //second nibble is channel
	//CHANNEL_VOICE_POLY_AFTERTOUCH = 0xA0, //second nibble is channel
	CHANNEL_VOICE_CONTROL_CHANGE = 0xB0, //second nibble is channel
	CHANNEL_VOICE_PROGRAM_CHANGE = 0xC0, //second nibble is channel
	//CHANNEL_VOICE_CHANNEL_AFTERTOUCH = 0xD0, //second nibble is channel
	//CHANNEL_VOICE_PITCH_BEND = 0xE0, //second nibble is channel

	//SYSTEM EXCLUSIVE
	SYSTEM_EXCLUSIVE_START = 0xF0,
	SYSTEM_EXCLUSIVE_END = 0xF7,

	//SYSTEM COMMON
	//SYSTEM_COMMON_MIDI_TIME_CODE_QUARTER_FRAME = 0xF1,
	//SYSTEM_COMMON_SONG_POSITION_POINTER = 0xF2,
	//SYSTEM_COMMON_SONG_SELECT = 0xF3,
	//SYSTEM_COMMON_TUNE_REQUEST = 0xF6,

	//SYSTEM REAL TIME
	SYSTEM_REAL_TIME_MIDI_CLOCK = 0xF8,
	SYSTEM_REAL_TIME_START = 0xFA,


	//needed for checking only
	SYSTEM_REAL_TIME_CONTINUE = 0xFB,
	//needed for checking only
	SYSTEM_REAL_TIME_STOP = 0xFC,
	//needed for checking only
	SYSTEM_REAL_TIME_ACTIVE_SENSING = 0xFE,



	SYSTEM_REAL_TIME_RESET = 0xFF
};

#endif




