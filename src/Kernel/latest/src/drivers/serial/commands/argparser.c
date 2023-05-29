#include <stdint.h>
#include "microclib/std.h"
#include "microclib/memcpy.h"

//Local prototypes
static int is_valid_number(char character);

//Signed 64 bit
int64_t argparser64(unsigned char* command, unsigned int arg_number, unsigned int base) {
/*
    Returns the argument at arg_number position in the command

    Arguments should only be separated by one space and not start with -

    Base 2 to 16 are supported.
    Negative numbers can only be used in base 10.

    Argument 0 is the first argument.

    Returns -1 on failure.
*/  

    //Skipping the command's name to the first argument
    while (*command != ' ' && *command != 0) command++;
    if(*command == 0) return -1;

    //Skipping the space character
    command++;

    //Skip the command's start to reach argument at position arg_number
    for (unsigned int i = 0; i < arg_number; i++) {
        while (*command != ' ' && *command != 0) command++; //Skip the arg to the next space char

        if(*command == 0) return -1;    //If we get a null terminator while skipping args it means
                                        //that the arg is missing.

        command++;  //Skip the space character
    }

    //Length (in characters) of the argument. Calculated to allocate the proper buffer size.
    int argLength = 0;
    for (int i = 0; is_valid_number(command[i]); i++) {
        argLength += 1;
    }
    if(argLength == 0) return -1;       //No argument ?

    //Add last space for null terminator
    unsigned char buffer[argLength + 1];
    buffer[argLength] = 0;  //Add null terminator
    
    //Copy the argument into the null terminated buffer
    memcpy(command, buffer, argLength);

    //Convert the argument to a number
    int64_t result = atoi(buffer, base);

    return result;
}

//Signed 32 bit
int32_t argparser(unsigned char* command, unsigned int arg_number, unsigned int base) {
    return (int32_t) argparser64(command, arg_number, base);
}

static int is_valid_number(char character) {
/*
    Function that returns 1 if the given characteer is between ASCII 0-9 A-F a-f or is '-'.
    Returns 0 otherwise.
*/
    if((character >= '0' && character <= '9') 
    || (character >= 'A' && character <= 'F')
    || (character >= 'a' && character <= 'f')
    || (character == '-'))
        return 1;
    
    return 0;
}

