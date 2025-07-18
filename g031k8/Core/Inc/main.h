#ifndef INC_MAIN_H_
#define INC_MAIN_H_

//INLCUDES
#include <stdint.h>
#include "config.h"
#include "oscillator.h"
#include "startup.h"
#include "LED.h"

//VARIABLE DECLARATIONS
volatile extern uint32_t statuses;
extern uint32_t midi_counter;
extern uint32_t idle_counter;
extern uint32_t preset_save_idle_counter;

#endif /* INC_MAIN_H_ */
