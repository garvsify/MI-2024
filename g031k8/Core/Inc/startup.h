#ifndef INC_STARTUP_H_
#define INC_STARTUP_H_

//INCLUDES
#include <stdint.h>

#include "oscillator.h"
#include "config.h"
#include "checking_functions.h"
#include "UART.h"
#include "input_capture.h"
#include "MIDI.h"

//FUNCTION DECLARATIONS
uint8_t __attribute__((optimize("O0")))Startup(void);

#endif /* INC_STARTUP_H_ */
