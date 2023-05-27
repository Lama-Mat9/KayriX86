#include <stdint.h>

#ifndef STD_H
#define STD_H

char* strrev(char* string);
unsigned int strlen(const char* string);

char* u64toa(uint64_t value, char* string, int base);
char* i64toa(int64_t value, char* string, int base);
char* utoa(uint32_t value, char* string, int base);
char* itoa(int32_t value, char* string, int base);

#endif
