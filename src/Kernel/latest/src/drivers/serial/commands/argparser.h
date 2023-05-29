#include <stdint.h>

#ifndef ARGPARSER_H
#define ARGPARSER_H

int64_t argparser64(unsigned char* command, unsigned int arg_number, unsigned int base);
int32_t argparser(unsigned char* command, unsigned int arg_number, unsigned int base);

#endif