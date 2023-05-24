#include <stdarg.h>
#include <stdint.h>
#include "microclib/std.h"
#include "microclib/stdio/vsprintf.h"
#include "microclib/stdio/formatted_size.h"

//Printing the string directly using the VGA driver.
//Meant as a temporary thing.
#include "drivers/screen/vga.h"

int vprintf(const char* format, va_list arguments) {
/*
    Wraps over vsprintf
    Prints formatted string
*/

    //  ---- Buffer allocation ----
    int buffer_size = formatted_size(format, arguments);

    //Once we have calculated the buffer size we can finally allocate it
    char buffer[buffer_size];

    //  ---- Formatting string ----

    //Send arguments to the function that we vrap over and keep return value.
    int return_value = vsprintf(buffer, format, arguments);

    // ---- Printing string ----

    for (unsigned int i = 0; i < strlen(buffer); i++) {
        VGA_print_char(buffer[i]);
    }

    return return_value;
}