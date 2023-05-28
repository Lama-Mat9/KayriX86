/*
	Serial port driver implementation
*/

#include <stdint.h>
#include <stdarg.h>
#include "serial.h"
#include "microclib/port_io.h"

#include "microclib/stdio/vsprintf.h"
#include "microclib/stdio/formatted_size.h"
#include "drivers/PIC/pic.h"
#include "microclib/memcmp.h"
#include "microclib/std.h"

#include "drivers/serial/commands/TABLES/TABLES.h"
#include "drivers/serial/commands/PIC/PIC.h"

//Internal prototypes
void serial_handle_command(unsigned char* command, uint16_t port);

int serial_init() {
/*
	Initialises serial ports defined in the header file.
	Returns the number of serial ports who were successfully initilised.
*/

	//How many ports we have tried to initialise.
	int tried_ports = 0;

//				---- Standard ports ----

	//Keeps track of the return status of each init attempt
	int status = 0;

	//Enable interrupts for registers that are usable and keep return codes
	int status_com1 = serial_init_port(SERIAL_IOPORT1);
	if(status_com1 == 0) PIC_IRQ_enable(4);
	else status += status_com1;

	int status_com2 = serial_init_port(SERIAL_IOPORT2);
	if(status_com2 == 0) PIC_IRQ_enable(3);
	else status += status_com2;

	//We keep the fact that we tried to initalise two ports
	tried_ports += 2;

//				---- Detected ports ----

	//Try to find more ports by reading from the Bios Data Area
	//Each port address should be one word long if it exists. Therefore 16 bit ptr used.
	uint16_t* data_ptr = (uint16_t*) 0x400;		//The address pointing to the start of the BDA

	//COM1 to COM4 could be present here.
	for (int i = 0; i < 4; i++) {

		//If there is a non null port address and we don't have it already
		if (*data_ptr != 0 && *data_ptr != SERIAL_IOPORT1 && *data_ptr != SERIAL_IOPORT2) {
			
			//We test the scratch register to make sure that this is a serial port
			
			//Write some random value to the scratch register
			portIO_byte_write((*data_ptr) + 7, 0xFA);

			//If when we read this register we get the same value that we wrote, we've found a valid port.
			if (portIO_byte_read((*data_ptr) + 7) == 0xFA) {
				status += serial_init_port(*data_ptr);
				tried_ports += 1;
			}	
		}

		data_ptr += 1;	//Move forward to the next entry.
	}
	
	//Number of tried ports - number of failed ports = the number of initialised ports
	return tried_ports - status;
}

int serial_init_port(uint16_t port) {
/*
	Sets up the serial port given in params for transmission.
*/

	//DLAB is 0, so this is the "Interrupt Enable Register".
	portIO_byte_write(port + 1, 0x00);	//Disable interrupts

	//Most significant bit of the Line Control register is DLAB
	portIO_byte_write(port + 3, 0x80); 	//Set DLAB to 1

	//DLAB is now 1, so this is the least significant byte for the baud rate divisor.
	portIO_byte_write(port + 0, 0x0C);	//115200 / 12 = 9600 baud rate

	//DLAB is 1, so this is the most significant byte for the baud rate divisor.
	portIO_byte_write(port + 1, 0x00);	//0x00 + 0x0c = 0x0c

	//Set DLAB back to 0 and select our options for the transfer
	portIO_byte_write(port + 3, 0x03);	//0x03 = 00000011b
		//Means no DLAB, no parity, one stop bit per char, 8bit data length
		//I chose 8 bit length because the smallest data types we have in C is 8bit.
		//Probably works in 7 bit for our use but never know.

	//Enable FIFO with a 14 byte threshold before data gets cleared.
	portIO_byte_write(port + 2, 0xC7);	///0xC7 = 11000111b

	//Request to send following data in loopback mode
	portIO_byte_write(port + 4, 0x1B);	//0x1E = 00011110b
		//We will simply send ourselves data to check if this works

	//Fill the data register with some data to send
	portIO_byte_write(port + 0, 0xFA);	//Randomly picked value

	//Now we receive the data that we just sent ourselves, read it and check
	//if the received value is same as the sent value to see if its working.
	
	if(portIO_byte_read(port + 0) != 0xFA) return 1;

	//Then if it works correctly we remove loopback, set DTR and RTS.
	//Set Auxiliary output 2 so that IRQs are enabled
	portIO_byte_write(port + 4, 0x0B);	//0x0B = 00001011b

	//DLAB set to 0, so this is Interrupt Enable Register
	portIO_byte_write(port + 1, 0x01);	//We set it to fire interrupt when some data is available

	
	//Print a message in the serial console to make sure it initialised
	serial_printf(port, "Serial connection initialised at port 0x{x}\f\r", port);

	return 0;
}

uint8_t is_transmitter_empty(uint16_t port) {
/*
	Returns 32 if transmitter is not doing anything. 0 if it is busy.
*/

	//The line status register (Offset 5) has it's 6th bit named "Transmitter empty"
	//that is set to 1 if the transmitter is not doing anything.

	//By ANDing the register with 0x20 (00100000b) we return the bit that we need directly.
	return portIO_byte_read(port + 5) & 0x20;
}

uint8_t is_data_ready(uint16_t port) {
/*
	Returns 1 if there is some data in the buffer that can be read. 0 otherwise.
*/

        //The line status register (Offset 5) has it's 1st bit named "Data ready"
        //that is set to 1 if there is some received data in the buffer that we could read.

	//By ANDIng the register with 0x01 (00000001b) we return the bit that we need directly.
	return portIO_byte_read(port + 5) & 0x01;
}

char serial_byte_read(uint16_t port) {
/*
	Reads one byte of received data from the serial port.
*/

	//While there is no data to read
	while(is_data_ready(port) == 0) {
		//We wait
	}

	return portIO_byte_read(port);
}

void serial_byte_write(uint16_t port, uint8_t byte) {
/*
	Sends a byte using the serial port.
*/

	//While the transmitter is busy
	while(is_transmitter_empty(port) == 0) {
		//We wait
	}

	//Now it should be ready so we send the data to port at offset 0 (transmit buffer)
	portIO_byte_write(port, byte);

}

void serial_printf(uint16_t port, const char* format, ...) {
/*
	Sends a formatted string using the serial port.
*/

	//	---- Formatting string ----

	//Lets us access each variadic argument
	va_list arguments;

	//Initialise arguments
	va_start(arguments, format);

	//Calculate required buffer size to store formatted string
	int buffer_size = formatted_size(format, arguments);

	//Allocate enough space for the formatted string
	char buffer[buffer_size];

	//Get the formatted string
	vsprintf(buffer, format, arguments);

	//Cleanup arguments
	va_end(arguments);

	//	---- Printing string ----

	int offset = 0;	//Pointer to where we are in the string

	//While the current char isnt the end of the string
	while(buffer[offset] != '\0') {
		serial_byte_write(port, buffer[offset]);	//We print the char
		offset += 1;	//Then continue to the next char
	}
}

void serial_handle_interrupt(uint16_t port) {
/*
	Handler for when IRQ3 or IRQ4 is triggered.
*/

	//Holds the characters sent through serial between now and the last time enter key was pressed.
	static unsigned char commandBuffer[201] = {0};	//All pure zeros
									//Last char reserved for null terminator

	//Read the byte that was sent to us
	char readByte = serial_byte_read(port);

	//Code for the "enter" key. Executes the command that was typed.
	if (readByte == 13)	{

		//New line
		serial_printf(port, "\f\r");

		//Do something with the command
		serial_handle_command(commandBuffer, port);

		//Flush the buffer
		for(unsigned int i = 0; i < sizeof(commandBuffer); i++) commandBuffer[i] = 0;
	}
	else {	//Behavior for any other keys

		//Print whatever was sent
		serial_printf(port, "{c}", readByte);

		//Find the last available space in the buffer and put received data inside of it
		for (unsigned int i = 0; i < sizeof(commandBuffer) - 1; i++) {	//Cannot overwrite null terminator

			if (commandBuffer[i] == 0) {		//We found an empty space
				
				commandBuffer[i] = readByte;	//Store the byte
				break;							//Stop searching
			}
		}
	}

}

void serial_handle_command(unsigned char* command, uint16_t port) {
/*
	Defines the behavior corresponding to each serial port command.

	Command has to be a null terminated string.
*/

	if (memcmp(command, "TABLES", strlen("TABLES")) == 0) {
		cmd_TABLES(port);
	}
	else if (memcmp(command, "PIC", strlen("PIC")) == 0) {
		cmd_PIC(port);
	} 		//Add commands here
	else {
		serial_printf(port, "Syntax error\f\r");
	}
	
}