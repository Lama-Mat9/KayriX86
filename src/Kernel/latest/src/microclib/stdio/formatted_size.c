#include <stdarg.h>
#include <stdint.h>
#include "microclib/get_digits.h"
#include "microclib/memcmp.h"
#include "microclib/stdio/string_flags.h"
#include "microclib/std.h"

enum ParseMode { NORMAL, FORMAT_SPECIFIER };

int formatted_size(const char* format, va_list arguments) {
/*
    Returns the expected length of a string after it would be formatted, using it's arguments.

    It has to behave in the same way as vsprintf, but without actually writing any data to a buffer.
*/

    va_list arguments_copy;
    va_copy(arguments_copy, arguments); //Get a new copy of arguments because we will expire it.

    int buffer_size = 0;    //Expected buffer size

    //Going to go state machine style
    enum ParseMode parse_mode = NORMAL;

    for (int i = 0; i <= (int) strlen(format); i++) {
        
        switch (format[i]) {
            case '{':

                //If we were already in format specifier mode when encountered it it will get printed
                if (parse_mode == FORMAT_SPECIFIER) buffer_size += 1;

                parse_mode = FORMAT_SPECIFIER;
                
                //If we were not in format specifier mode when encountered it it wont get printed

                continue;
            case '}':

                //If we are not in format specifier mode it will get printed
                if (parse_mode == NORMAL) buffer_size += 1;

                parse_mode = NORMAL;
                
                //If we were in format specifier mode it won't get printed

                continue;
        }

        if (parse_mode == FORMAT_SPECIFIER) {
            switch (format[i]) {
                case 'd':
                case 'o':
                case 'b':
                case 'x': {       // int argument expected (different bases)

                    //If the size parameter is included
                    if(format[i+1] == '.') {
                        if (memcmp((char*) format + i + 2, "i32", 3) == 0) {           //int32
                            int base = flag_to_base(format[i]); //Get the base corresponding to given flag
                            int32_t argument = va_arg(arguments_copy, int32_t);  //Get the arg to print (variadic)
                            unsigned int digits = get_digits_signed(argument, base);    //Count the digits
                            if(argument < 0) digits++;      //Could need one more space for '-'
                            buffer_size += digits;
                        }
                        else if (memcmp((char*) format + i + 2, "i64", 3) == 0) {      //int64
                            int base = flag_to_base(format[i]);
                            int64_t argument = va_arg(arguments_copy, int64_t);
                            unsigned int digits = get_digits_signed(argument, base);
                            if(argument < 0) digits++;
                            buffer_size += digits;
                        }
                        else if (memcmp((char*) format + i + 2, "u32", 3) == 0) {      //uint32
                            int base = flag_to_base(format[i]);
                            uint32_t argument = 0;
                            argument = va_arg(arguments_copy, uint32_t);
                            unsigned int digits = get_digits_unsigned(argument, base);
                            buffer_size += digits;
                        }
                        else if (memcmp((char*) format + i + 2, "u64", 3) == 0) {      //uint64
                            int base = flag_to_base(format[i]);
                            uint64_t argument = va_arg(arguments_copy, uint64_t);
                            unsigned int digits = get_digits_unsigned(argument, base);
                            buffer_size += digits;
                        }

                        i += 4;     //Skip all that we've read
                        
                    }
                    else {  //The size parameter was not included. Assuming i32.
                        int base = flag_to_base(format[i]);
                        int32_t argument = va_arg(arguments_copy, int32_t);
                        unsigned int digits = get_digits_signed(argument, base);
                        if(argument < 0) digits++;
                        buffer_size += digits;
                    }
                
                    break;
                }
                case 'c': {     //char argument expected

                    //Getting the argument to not break everything, but unnecessary
                    va_arg(arguments_copy, int);
                    
                    buffer_size += 1;

                    break;
                }
                case 's': {     //char* argument expected

                    //Getting the argument
                    char* argument = va_arg(arguments_copy, char*);

                    //Get amount of chars to copy
                    int argument_length = strlen(argument);

                    buffer_size += argument_length;

                    break;
                }
                default: {  //Format character not understood

                    //Character would get printed
                    buffer_size += 1;

                    break;
                }
            }
        }
        else {
            
            //In case we aren't in format specifier mode a character will get printed
            buffer_size += 1;
        }
    }

    // ---- Cleanup ----

    //We have expired our local copy of arguments.
    va_end(arguments_copy);

    return buffer_size;
}