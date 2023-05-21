#include <stddef.h>

void* memset(void* address, int value, size_t data_length) {
/*
	Sets the first data_length bytes of memory
	at the address pointed to by argument address
	to the provided value.

	Returns the same address that it was provided.

	Note: value is an int only because we need to keep compatibility
	with the C standard. Only the least significant byte of value
	will be used.
*/

	//We need a char sized pointer to perform byte sized operations.
	unsigned char* address_copy = address;	//Also keep address for returning

	//While we haven't copied all bytes
	while(data_length != 0) {

		//This cast only keeps the least significant byte of value
		*address_copy = (unsigned char) value;

		//Next byte to copy
		address_copy += 1;

		//One byte less to copy
		data_length -= 1;
	}

	return address;
}
