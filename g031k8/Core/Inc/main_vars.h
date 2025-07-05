#ifndef INC_MAIN_VARS_H_
#define INC_MAIN_VARS_H_

//VARIABLE DEFINITIONS
volatile uint32_t statuses = 0; //set of all status bits (to reduce memory usage)
uint32_t idle_counter = 0;
uint32_t midi_counter = 0;
uint32_t advance_idle_counter = 0;

#endif /* INC_MAIN_VARS_H_ */
