#include <stdint.h>

#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

uint8_t PS2kb_readScancode();
uint8_t PS2kb_readStatusBit(int bitNumber);

#endif