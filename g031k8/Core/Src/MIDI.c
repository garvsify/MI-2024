//INCLUDES
#include "MIDI.h"

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
