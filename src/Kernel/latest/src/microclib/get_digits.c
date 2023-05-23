#include <stdint.h>

int get_digits_signed(int32_t value, int base) {
/*
    Returns how many digits there are in a signed integer.
*/

    //Special case when value is 0 because it is 1 digit
    if(value == 0) return 1;

    int digits = 0;
    
    while (value != 0) {
        value /= base;
        digits += 1;
    }
    
    return digits;
}

int get_digits_unsigned(uint32_t value, int base) {
/*
    Returns how many digits there are in an unsigned integer.
*/

    //Special case when value is 0 because it is 1 digit
    if(value == 0) return 1;

    int digits = 0;
    
    while (value != 0) {
        value /= base;
        digits += 1;
    }
    
    return digits;
}