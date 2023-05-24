#include <stdarg.h>
#include "microclib/stdio/vsprintf.h"

int sprintf(char* buffer, const char* format, ...) {
/*
    Wrapper for vsprintf.
    Its variadic arguments are passed along to vsprintf.
*/

    //Keeps the arguments passed as parameters
    va_list arguments;

    //Required to let me start fetching the arguments
    va_start(arguments, format);

    //Send arguments to the function that we vrap over and keep return value.
    int return_value = vsprintf(buffer, format, arguments);

    //Cleanup
    va_end(arguments);

    return return_value;
}