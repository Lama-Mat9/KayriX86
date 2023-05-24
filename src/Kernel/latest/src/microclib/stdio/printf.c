#include <stdarg.h>
#include "microclib/stdio/vprintf.h"

int printf(const char* format, ...) {
/*
    Wraps over vprintf
    It's variadic arguments are passed along to vprintf
*/

    //Keeps the arguments passed as parameters
    va_list arguments;

    //Required to let me start fetching the arguments
    va_start(arguments, format);

    //Send arguments to the function that we vrap over and keep return value.
    int return_value = vprintf(format, arguments);

    //Cleanup
    va_end(arguments);

    return return_value;
}