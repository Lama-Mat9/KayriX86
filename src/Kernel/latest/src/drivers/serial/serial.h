
#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

//Standard port IO address for the COM1 port
#define SERIAL_IOPORT 0x3f8	//Could be set to use other serial ports by setting their port address

int serial_init();
void serial_string_write(char* string);
void serial_byte_write(uint8_t byte);
uint8_t is_transmitter_empty();
uint8_t is_data_ready();
char serial_byte_read();

#endif
