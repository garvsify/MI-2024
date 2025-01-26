#ifndef SRC_UTILITY_H_
#define SRC_UTILITY_H_

//MACROS
#define ON 1
#define OFF 0

//INCLUDES
#include <stdint.h>
#include "config.h"

//CUSTOM TYPES
enum Polarity{
	NEGATIVE,
	POSITIVE
};

enum Validate{
	NO,
	YES
};

//FUNCTION DECLARATIONS
enum Validate isPrime(uint16_t x);
uint32_t unsigned_bitwise_modulo(uint32_t dividend, uint8_t base_2_exponent);
uint8_t Global_Interrupt_Enable(void);
uint8_t Global_Interrupt_Disable(void);

#endif /* SRC_UTILITY_H_ */
