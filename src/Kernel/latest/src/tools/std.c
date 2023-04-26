/*
	Manual implementation of standard C functions needed while we have no access to a C library yet.
*/

//		---- Internal function prototypes ----
unsigned int strlen(const char* string);
char* strrev(char* string);
char* itoa(int value, char* string, int base);

char* itoa(int value, char* string, int base) {
/*
	Converts an integer value to a null-terminated string using the specified base,
	then stores the result in the array given by string parameter.

	Returns the same address as the given string parameter.
*/
	//The index that we will use through this function to write to input string.
	int index = 0;

	//We have no booleans so we use an unsigned char
	unsigned char isNegative = 0;

	//Zero has to be handled explicitly.
	//Else empty string would get printed.
	if(value == 0) {
		string[index] = '0';
		string[index + 1] = '\0';
		return string;
	}

	//Like the real itoa(), negative numbers are handled only in base 10.
	//Otherwise they are considered unsigned.
	if(base == 10 && value < 0) {
		value = value * -1;	//We have to do our calculations on a positive number
		isNegative = 1;		//We keep the fact that it was negative though
	}


	//Dividing each number by it's base gives us the least significant digit.
	while(value != 0) {
		int remainder = value % base; 	//Least significant digit

		//Converting non numbers to letters by adding their offset in the ASCII table to ASCII 'a'
		if(remainder > 9) string[index] = 'a' + (remainder - 10);
		else string[index] = '0' + remainder;	//Converting integers to ASCII numbers by adding ASCII 0

		index += 1;
		value = value / base;	//We do that so that next iteration gets the next least significant digit.
	}



	//Mark that the number was negative if it was.
	//We put it at the end because the array will get flipped
	if (isNegative) {
		string[index] = '-';
		index += 1;
	}

	//Need to add string terminator because we've made the string ourselves.
	string[index] = '\0';
	index += 1;

	//Reverse the string
	strrev(string);

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
