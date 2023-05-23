#include <stdarg.h>
#include <stdint.h>
#include "microclib/std.h"
#include "microclib/get_digits.h"
#include "microclib/memcpy.h"

static int flag_to_base(char format);

enum ParseMode { NORMAL, FORMAT_SPECIFIER };

int sprintf(char* buffer, const char* format, ...) {
/*
    Non-standard implementation of sprintf
    Each format specifier should be preceded by { and followed by }
    Returns null terminated formatted string

    Supported format codes: d o b x c s
*/

    //Going to go state machine style
    enum ParseMode parse_mode = NORMAL;

    //Keeps the arguments passed as parameters
    va_list arguments;

    //Keeps our current position in the buffer
    uint32_t buffer_index = 0;

    //Required to let me start fetching the arguments
    va_start(arguments, format);

    //Go through the entire format string char per char
    for (uint32_t i = 0; i <= strlen(format); i++) {
        
        // ---- STATE MACHINE MANAGEMENT ----

        switch (format[i]) {
        case '{':
            parse_mode = FORMAT_SPECIFIER;  //Makes it so that next char read will
                                            //be expected to be a format specifier
            continue;                        
    
        case '}':
            parse_mode = NORMAL;    //Next char will be expected to be a printable char again
            
            continue;
        }
        
        // ---- PARSING ----

        if (parse_mode == FORMAT_SPECIFIER) {   //Only parse format specifier after opening bracket
            switch (format[i]) {
                case 'd':
                case 'o':
                case 'b':
                case 'x': {       // int argument expected (different bases)

                    //This case handles multiple possible bases at the same time
                    int base = flag_to_base(format[i]);

                    //Getting the argument
                    int32_t argument = va_arg(arguments, int32_t);

                    //To determine the amount of space we need to store the number we
                    //have to determine how many digits are in the number.
                    int digits = get_digits_signed(argument, base);
                    if(argument < 0) digits++;  //We need another char for '-' if negative

                    //Allocate space to store the char* version of the number.
                    //Add 1 more for \0
                    char tmp_buffer[digits + 1];

                    //Store the number inside of the temporary buffer
                    itoa(argument, tmp_buffer, base);

                    //Store the temporary buffer (without \0) at the end of the real buffer
                    memcpy(tmp_buffer, &(buffer[buffer_index]), digits);

                    //Update our write position in the buffer to prevent overwriting what we just wrote
                    buffer_index += digits;

                    break;
                }
                case 'c': {     //char argument expected

                    //Getting the argument
                    char argument = (char) va_arg(arguments, int);
                    
                    buffer[buffer_index] = argument;

                    buffer_index++;

                    break;
                }
                case 's': {     //char* argument expected

                    //Getting the argument
                    char* argument = va_arg(arguments, char*);

                    //Get amount of chars to copy
                    uint32_t argument_length = strlen(argument);

                    //Copy all chars without null terminator
                    memcpy(argument, &(buffer[buffer_index]), argument_length); 

                    buffer_index += argument_length;

                    break;
                }
            }
            
            
        }
        else if (parse_mode == NORMAL) {      //Current character is expected to be printed
            buffer[buffer_index] = format[i];
            buffer_index++;
        }
            
    }
    
    //Cleanup the arguments
    va_end(arguments);

    //In case the format string wasnt null terminated, means our formatted string is not either. 
    if (format[strlen(format)] != '\0') {
        buffer[buffer_index] = '\0';
    }
    else {  //If it was null terminated
        buffer_index--;     // \0 does not count in the amount of characters written.
    }
    
    //The buffer index also corresponds to how many characters we have written
    return buffer_index;
}

static int flag_to_base(char format) {
/*
    Returns base of formats d, x, o, b 
*/

    switch (format) {
        case 'x':
            return 16;
        case 'd':
            return 10;
        case 'o':
            return 8;
        case 'b':
            return 2;
    }

    //For security, but shouldnt be triggered.
    return 10;
}