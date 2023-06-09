/*
	Manual implementation of standard C functions needed while we have no access to a C library yet.
*/

#include <stdbool.h>
#include <stdint.h>
#include "std.h"

//Local prototypes
static int char_to_int(char character);

static char* x64toa(uint64_t value, char* string, unsigned int base, unsigned int isNegative) {
/*
	Converts a 64bit unsigned integer value to a null-terminated string using the specified base,
	then stores the result in the array given by string parameter.

	As it only works on unsigned integers, if the number was negative you need to pass
	that information in the parameters and provide the absolute value of your number.

	Returns the same address as the given string parameter.
*/

	//The index that we use to write to write to input string
	int index = 0;

	//Zero has to be handled explicitly.
	//Else empty string would get printed.
	if(value == 0) {
		string[index] = '0';
		string[index + 1] = '\0';
		return string;
	}

	//Will store each digit that we extract from the value one by one.
	int currentDigit = 0;

	while (value != 0) {
		currentDigit = (unsigned int) (value % base);	//Extract least significant digit

		value /= base;					//Remove least significant digit from value

		//Converting non numbers to letters by adding their offset in the ASCII table to ASCII 'a'
		if(currentDigit > 9) string[index] = 'a' + (currentDigit - 10);
		else string[index] = '0' + currentDigit;	//Converting integers to ASCII numbers by adding ASCII 0

		index += 1;
	}
	
	//If it was negative we end the string with a '-'
	if(isNegative){
		string[index] = '-';
		index += 1;
	}

	//Need to add string terminator because we've made the string ourselves.
	string[index] = '\0';
	index += 1;
	
	strrev(string);

	return string;
}

//Signed 64bit
char* i64toa(int64_t value, char* string, int base) { //Negative values are handled only in base 10
	if(value < 0) x64toa((uint64_t) value * -1, string, base, (base == 10));
	else x64toa((uint64_t) value, string, base, 0);
	return string;
}

//Unsigned 64bit
char* u64toa(uint64_t value, char* string, int base) {
	x64toa(value, string, base, 0);
	return string;
}

//Signed 32bit
char* itoa(int32_t value, char* string, int base) {	//Negative values are handled only in base 10
	if(value < 0) x64toa((uint64_t) value * -1, string, base, (base == 10));
	else x64toa((uint64_t) value, string, base, 0);
	return string;					
}

//Unsigned 64bit
char* utoa(uint32_t value, char* string, int base) {
	x64toa((uint64_t) value, string, base, 0);
	return string;
}

char* strrev(char* string) {
/*
	Reverses the given string.

	Returns the same address as the given parameter.
*/
	//Index of the character at the start of the string that will be swapped.
	int start = 0;

	//Index of the character at the end of the string that will be swapped.
	int end = strlen(string) - 1;

	//While we haven't swapped everything
	while(start < end) {
		char tmp = string[start];	//Keep start's value
		string[start] = string[end];	//so that we can overwrite it with end's value
		string[end] = tmp;		//Then write start to the end. Effectively swapping start and end.
		end -= 1;			//Next char to swap.
		start += 1;
	}

	return string;
}


unsigned int strlen(const char* string) {
/*
	Returns the length of a null-terminated string.
*/
	unsigned int count = 0;

	while(*string != '\0') {
		count++;
		string++;
	}

	return count;
}

int64_t atoi(unsigned char* ptr, uint32_t base) {
/*
	Extracts an integer (positive or negative) from the given string using given base.

	The given string has to be null terminated.
*/

	//Keeps our result
    uint64_t result = 0;

	//Positive if number is positive, negative otherwise
    int sign = 1;
    if(*ptr == '-') {
        sign = -1;
        ptr++;
    }

	//Stop when \0 is found
    for(int i = 0; *(ptr + i) != '\0'; i++) {
        result = (result * base) + char_to_int(*(ptr+i));
    }

	//Make the result negative if it was negative in the string. Stays positive otherwise.
    result *= sign;

    return result;
}

static int char_to_int(char character) {
/*
    Converts an ASCII character A-F a-F 0-9 to an integer.
*/
    if(character >= '0' && character <= '9') return (int) character - '0';
    else if (character >= 'A' && character <= 'F') return (int) character - 'A' + 10;
    else if (character >= 'a' && character <= 'f') return (int) character - 'a' + 10;

    return 0;
}
