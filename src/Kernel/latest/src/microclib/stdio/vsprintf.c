#include <stdarg.h>
#include <stdint.h>
#include "microclib/std.h"
#include "microclib/get_digits.h"
#include "microclib/memcpy.h"
#include "microclib/stdio/flag_to_base.h"


enum ParseMode { NORMAL, FORMAT_SPECIFIER };

int vsprintf(char* buffer, const char* format, va_list arguments) {
/*
    Non-standard implementation of vsprintf
    Each format specifier should be preceded by { and followed by }
    Example format string: "There are {d} dragons."

    Format has to be a null terminated string.
    Returns null terminated formatted string

    Supported format codes: d o b x c s
    All of the supported format codes work the same way as standard ANSI C

    Entering escape brackets inside of escape brackets will print only the internal escape brackets
    Example format string: "Hello {{}}"
    Gets formatted to: "Hello {}"

    Need to modify formatted_bufsize if we modify vsprintf's behavior.
    This is because formatted_bufsize is guessing the buffer size required to use this function.
*/

    //Going to go state machine style
    enum ParseMode parse_mode = NORMAL;

    //Keeps our current position in the buffer
    int buffer_index = 0;

    //Go through the entire format string char per char
    for (int i = 0; i <= (int) strlen(format); i++) {
        
        // ---- STATE MACHINE MANAGEMENT ----

        switch (format[i]) {
            case '{':

                //If we are already in escaped mode and find a open escape mode character,
                //prevent skip printing it
                if(parse_mode == FORMAT_SPECIFIER) break;

                //Next character will be interpreted as format specifier
                parse_mode = FORMAT_SPECIFIER;

                continue;        
            
            case '}':

                //If we aren't in escaped mode and find and find the close escaped mode character
                //prevent skip printing it
                if(parse_mode == NORMAL) break;
                
                //Next char will be expected to be a printable char again
                parse_mode = NORMAL;    
                
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
                    int argument_length = strlen(argument);

                    //Copy all chars without null terminator
                    memcpy(argument, &(buffer[buffer_index]), argument_length); 

                    buffer_index += argument_length;

                    break;
                }
                default: {  //Format character not understood

                    //We print it
                    buffer[buffer_index] = format[i];
                    buffer_index++;

                    break;   
                }
            }
            
            
        }
        else if (parse_mode == NORMAL) {      //Current character is expected to be printed
            buffer[buffer_index] = format[i];
            buffer_index++;
        }
            
    }

    // \0 was copied from format string too.
    buffer_index--;     // \0 does not count in the amount of characters written.

    //The buffer index also corresponds to how many characters we have written
    return buffer_index;
}