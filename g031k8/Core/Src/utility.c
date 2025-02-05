#include "utility.h"

enum Validate isPrime(uint16_t x){
    for (uint16_t d = 2; d * d <= x; d++) {
        if (x % d == 0)
            return NO;
    }
    return YES;
}

uint32_t unsigned_bitwise_modulo(uint32_t dividend, uint8_t base_2_exponent){

    return dividend & ((1 << base_2_exponent) - 1);
}

uint8_t Global_Interrupt_Enable(void){

	__enable_irq();
	return 1;
}

uint8_t Global_Interrupt_Disable(void){

	__disable_irq();
	return 1;
}

enum Validate Get_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit){

    if(*bits & bit){

        return (enum Validate) YES;
    }

    else{

        return (enum Validate) NO;
    }
}

void Set_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit){

	*bits |= bit;
}

void Clear_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit){

	*bits &= ~bit;
}
