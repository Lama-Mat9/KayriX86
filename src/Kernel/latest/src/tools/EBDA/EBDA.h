#include <stdint.h>

#ifndef EBDA_H
#define EBDA_H

//Provide to all source files the local EBDA address
extern char* EBDA;

int EBDA_init();

#endif