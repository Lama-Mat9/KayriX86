#include <stdint.h>

#ifndef GET_DIGITS_H
#define GET_DIGITS_H

int get_digits_signed(int64_t value, int base);
int get_digits_unsigned(uint64_t value, int base);

#endif