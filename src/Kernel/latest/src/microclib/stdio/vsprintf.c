#include <stdarg.h>
#include <stdint.h>
#include "microclib/std.h"
#include "microclib/get_digits.h"
#include "microclib/memcpy.h"
#include "microclib/memcmp.h"
#include "microclib/stdio/string_flags.h"


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

    You can specify size and signedness by appending .u32 .u64 .i32 or .i64 to the format code
    Example format string: "Unsigned 32 bit pointer here: {x.u32}"

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
                case 'x': {
                    
                    //If the size parameter is included
                    if(format[i+1] == '.') {
                        if (memcmp((char*) format + i + 2, "i32", 3) == 0) {           //int32
                            int base = flag_to_base(format[i]); //Get the base corresponding to given flag
                            int32_t argument = va_arg(arguments, int32_t);  //Get the arg to print (variadic)
                            unsigned int digits = get_digits_signed(argument, base);    //Count the digits
                            if(argument < 0) digits++;      //Could need one more space for '-'
                            char tmp_buffer[digits + 1];    //Need one more space for '\0' 
                            itoa(argument, tmp_buffer, base);   //Convert to char*
                            memcpy(tmp_buffer, &(buffer[buffer_index]), digits);
                            buffer_index += digits;
                        }
                        else if (memcmp((char*) format + i + 2, "i64", 3) == 0) {      //int64
                            int base = flag_to_base(format[i]);
                            int64_t argument = va_arg(arguments, int64_t);
                            unsigned int digits = get_digits_signed(argument, base);
                            if(argument < 0) digits++;
                            char tmp_buffer[digits + 1];
                            i64toa(argument, tmp_buffer, base);
                            memcpy(tmp_buffer, &(buffer[buffer_index]), digits);
                            buffer_index += digits;
                        }
                        else if (memcmp((char*) format + i + 2, "u32", 3) == 0) {      //uint32
                            int base = flag_to_base(format[i]);
                            uint32_t argument = 0;
                            argument = va_arg(arguments, uint32_t);
                            unsigned int digits = get_digits_unsigned(argument, base);
                            char tmp_buffer[digits + 1];
                            utoa(argument, tmp_buffer, base);
                            memcpy(tmp_buffer, &(buffer[buffer_index]), digits);
                            buffer_index += digits;
                        }
                        else if (memcmp((char*) format + i + 2, "u64", 3) == 0) {      //uint64
                            int base = flag_to_base(format[i]);
                            uint64_t argument = va_arg(arguments, uint64_t);
                            unsigned int digits = get_digits_unsigned(argument, base);
                            char tmp_buffer[digits + 1];
                            u64toa(argument, tmp_buffer, base);
                            memcpy(tmp_buffer, &(buffer[buffer_index]), digits);
                            buffer_index += digits;
                        }

                        i += 4;     //Skip all that we've read
                        
                    }
                    else {  //The size parameter was not included. Assuming i32.
                        int base = flag_to_base(format[i]);
                        int32_t argument = va_arg(arguments, int32_t);
                        unsigned int digits = get_digits_signed(argument, base);
                        if(argument < 0) digits++;
                        char tmp_buffer[digits + 1];
                        itoa(argument, tmp_buffer, base);
                        memcpy(tmp_buffer, &(buffer[buffer_index]), digits);
                        buffer_index += digits;
                    }
                
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