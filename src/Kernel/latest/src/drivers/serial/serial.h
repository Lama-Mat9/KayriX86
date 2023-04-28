
#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

//Standard port IO address for the COM1 port
#define SERIAL_IOPORT1 0x3f8			//Could be set to use
						//other serial ports
//Standard port IO address for the COM2 port	//by setting
#define SERIAL_IOPORT2 0x2f8			//their port address

int serial_init();
int serial_init_port(uint16_t port);
void serial_string_write(uint16_t port, char* string);
void serial_byte_write(uint16_t port, uint8_t byte);
uint8_t is_transmitter_empty(uint16_t port);
uint8_t is_data_ready(uint16_t port);
char serial_byte_read(uint16_t port);

#endif
