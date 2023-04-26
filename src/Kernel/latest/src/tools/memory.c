/*
	Defines helpful functions for memory manipulation.
*/

//We include this to get access to size_t
#ifndef STDDEF
#define STDDEF
#include <stddef.h>
#endif

//		---- Internal function prototypes ----
void memcopy(void* source, const void* destination, size_t byte_amount);

void memcpy(void* source, const void* destination, size_t byte_amount) {
/*
	Function that copies byte_amount amount of bytes from source to destination address.

	Notes: We are using void pointers so that any pointer types should work.
	We use char* pointers because byte per byte copy, so if a particular data type is used
	it will work because it's always bigger than a char*.

	One issue is that void pointers can't be dereferenced (else how do we guess the size of
	the data we access). This is why we cast the pointer to a type (char*) for 1 byte access.

	We are using unsigned longs because that's what a 32 bit processor should allow us to use at most.
	We dont' want to be limited to an int in terms of bytes to copy. Though allocating 32 bits each copy
	seems unoptimized. Maybe fix this later.
*/
	//Byte per byte copy of the source.
	for(size_t i = 0; i < byte_amount; i++) {
		*((char*) destination + i) = *((char*) source + i);
	}

}
