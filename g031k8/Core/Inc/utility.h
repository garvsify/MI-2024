#ifndef SRC_UTILITY_H_
#define SRC_UTILITY_H_

//MACROS
#define ON 1
#define OFF 0

//INCLUDES
#include <stdint.h>
#include "input_capture.h"
#include "config.h"
#include "oscillator.h"
#include "oscillator_macros.h"

//CUSTOM TYPES
enum Polarity{
	NEGATIVE,
	POSITIVE
};

enum Validate{
	NO,
	YES
};

enum Status_Bit{
	Input_Capture_Processing_Can_Be_Started = INPUT_CAPTURE_PROCESSING_CAN_BE_STARTED,
	First_Sync_Complete = FIRST_SYNC_COMPLETE,
	Initial_ADC_Conversion_Complete = INITIAL_ADC_CONVERSION_COMPLETE
};

//FUNCTION DECLARATIONS
enum Validate isPrime(uint16_t x);
uint32_t unsigned_bitwise_modulo(uint32_t dividend, uint8_t base_2_exponent);
uint8_t Global_Interrupt_Enable(void);
uint8_t Global_Interrupt_Disable(void);
enum Validate Get_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit);
void Set_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit);
void Clear_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit);

#endif /* SRC_UTILITY_H_ */
