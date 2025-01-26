#include "utility.h"

enum Validate isPrime(uint16_t x){
    for (int d = 2; d * d <= x; d++) {
        if (x % d == 0)
            return NO;
    }
    return YES;
}

uint32_t unsigned_bitwise_modulo(uint32_t dividend, uint8_t base_2_exponent){

    return dividend & ((1 << base_2_exponent) - 1);
}
