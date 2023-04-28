/*
	Serial port driver implementation
*/

#include "serial.h"
#include "../../tools/port_io.h"
#include <stdint.h>

int serial_init() {
/*
	Sets up the serial port defined in the header file for transmission.
*/

	//DLAB is 0 by default, so this is the "Interrupt Enable Register".
	portIO_byte_write(SERIAL_IOPORT + 1, 0x00);	//Disable interrupts.

	//Most significant bit of the Line Control register is DLAB
	portIO_byte_write(SERIAL_IOPORT + 3, 0x80); 	//Set DLAB to 1

	//DLAB is now 1, so this is the least significant byte for the baud rate divisor.
	portIO_byte_write(SERIAL_IOPORT + 0, 0x0c);	//115200 / 12 = 9600 baud rate

	//DLAB is 1, so this is the most significant byte for the baud rate divisor.
	portIO_byte_write(SERIAL_IOPORT + 1, 0x00);	//0x00 + 0x0c = 0x0c

	//Set DLAB back to 0 and select our options for the transfer
	portIO_byte_write(SERIAL_IOPORT + 3, 0x03);	//0x02 = 00000010b
		//Means no DLAB, no parity, one stop bit per char, 8bit data length
		//I chose 8 bit length because the smallest data types we have in C is 8bit.
		//Probably works in 7 bit for our use but never know.

	//Enable FIFO with a 14 byte threshold before data gets cleared.
	portIO_byte_write(SERIAL_IOPORT + 2, 0xC7);	///0xC7 = 11000111b

	//Handshaking. Requests for modem so send, sets ready, IRQ enabled
	portIO_byte_write(SERIAL_IOPORT + 4, 0x0B);	//0x0B = 00001011b

	//Request to send following data in loopback mode
	portIO_byte_write(SERIAL_IOPORT + 4, 0x1E);	//0x1E = 00011110b
		//We will simply send ourselves data to check if this works

	//Fill the data register with some data to send
	portIO_byte_write(SERIAL_IOPORT + 0, 0xFA);	//Randomly picked value


	//Now we receive the data that we just sent ourselves, read it and check
	//if the received value is same as the sent value to see if its working.
	if(portIO_byte_read(SERIAL_IOPORT + 0) != 0xFA) {
		return 1;
	}

	//Then if it works correctly we remove loopback, enable outputs and set ready
	portIO_byte_write(SERIAL_IOPORT + 4, 0x0F);	//0x0F = 00001111b

	//Print a message in the serial console to make sure it initialised
	char* msg = "Serial connection initialised\f\r";
	serial_string_write(msg);

	return 0;
}

uint8_t is_transmitter_empty() {
/*
	Returns 32 if transmitter is not doing anything. 0 if it is busy.
*/

	//The line status register (Offset 5) has it's 6th bit named "Transmitter empty"
	//that is set to 1 if the transmitter is not doing anything.

	//By ANDing the register with 0x20 (00100000b) we return the bit that we need directly.
	return portIO_byte_read(SERIAL_IOPORT + 5) & 0x20;
}

uint8_t is_data_ready() {
/*
	Returns 1 if there is some data in the buffer that can be read. 0 otherwise.
*/

        //The line status register (Offset 5) has it's 1st bit named "Data ready"
        //that is set to 1 if there is some received data in the buffer that we could read.

	//By ANDIng the register with 0x01 (00000001b) we return the bit that we need directly.
	return portIO_byte_read(SERIAL_IOPORT + 5) & 0x01;
}

char serial_byte_read() {
/*
	Reads one byte of received data from the serial port.
*/

	//While there is no data to read
	while(is_data_ready() == 0) {
		//We wait
	}

	return portIO_byte_read(SERIAL_IOPORT);
}

void serial_byte_write(uint8_t byte) {
/*
	Sends a byte using the serial port.
*/

	//While the transmitter is busy
	while(is_transmitter_empty() == 0) {
		//We wait
	}

	//Now it should be ready so we send the data to port at offset 0 (transmit buffer)
	portIO_byte_write(SERIAL_IOPORT, byte);

}

void serial_string_write(char* string) {
/*
	Sends a string using the serial port.
*/

	//While the current char isnt the end of the string
	while(*string != '\0') {
		serial_byte_write(*string);	//We print the char
		string += 1;	//Then continue to the next char
	}
}
