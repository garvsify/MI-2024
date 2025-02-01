#ifndef INC_SYMMETRY_ADJUSTMENT_H_
#define INC_SYMMETRY_ADJUSTMENT_H_

//INCLUDES
#include <oscillator_params.h>
#include <stdint.h>

#include "symmetry_adjustment_types.h"
#include "oscillator.h"

//CUSTOM TYPES


//FUNCTION DECLARATIONS
uint8_t Adjust_TIM16_Prescaler(struct Params* params_ptr);
uint8_t Process_TIM16_Final_Start_Value_and_Final_Prescaler(struct Params* params_ptr);

#endif /* INC_SYMMETRY_ADJUSTMENT_H_ */
