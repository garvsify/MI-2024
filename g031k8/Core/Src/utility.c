#include "utility.h"

enum Validate __attribute__((optimize("O0")))isPrime(uint16_t x){
    for (uint16_t d = 2; d * d <= x; d++) {
        if (x % d == 0)
            return NO;
    }
    return YES;
}

uint32_t __attribute__((optimize("O0")))unsigned_bitwise_modulo(uint32_t dividend, uint8_t base_2_exponent){

    return dividend & ((1 << base_2_exponent) - 1);
}

uint8_t __attribute__((optimize("O0")))Global_Interrupt_Enable(void){

	__enable_irq();
	return 1;
}

uint8_t __attribute__((optimize("O0")))Global_Interrupt_Disable(void){

	__disable_irq();
	return 1;
}

enum Validate __attribute__((optimize("O0")))Get_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit){

    if(*bits & bit){

        return (enum Validate) YES;
    }

    else{

        return (enum Validate) NO;
    }
}

void __attribute__((optimize("O0")))Set_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit){

	*bits |= bit;
}

void __attribute__((optimize("O0")))Clear_Status_Bit(volatile uint32_t *bits, enum Status_Bit bit){

	*bits &= ~bit;
}
