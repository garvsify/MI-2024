#ifndef SRC_MIDI_H_
#define SRC_MIDI_H_

//MACROS

//INCLUDES
#include <stdint.h>
#include "utility.h"

//CUSTOM TYPES

//DECLARATIONS
enum Validate Is_Status_Byte(volatile char *data);
enum Validate Is_Program_Change_Data_Byte_In_Range(volatile char *PC_data, uint8_t size_of_factory_or_user_array);
enum Validate Is_System_Real_Time_Status_Byte(volatile char *data);

#endif /* SRC_MIDI_H_ */
