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
extern uint32_t preset_save_select_idle_counter;
extern uint32_t led_pause_counter;
volatile extern enum Validate save_or_preset_mode_engaged;

#endif /* INC_MAIN_H_ */
