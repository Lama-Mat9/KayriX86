/*
    Definition of MEMSHOW command
*/

#include <stdint.h>
#include "drivers/serial/serial.h"
#include "microclib/std.h"

#define BYTES_PER_LINE 8

void cmd_MEMSHOW(uint16_t port, char* start_addr, unsigned int byte_len) {
/*
    Displays memory contents at given location.
    Starts displaying contents at start_addr, and displays byte_len bytes.
*/

    serial_printf(port, "Address: 0x{x} Content length: {d.u32}\f\r", start_addr, byte_len);

    //Keeps the characters that were printed on one line
    char buffer[BYTES_PER_LINE + 1];
    buffer[BYTES_PER_LINE] = 0;         //Null terminator (never know)
    int byte_number = 0;        //The index of the character we are printing on the current line

    while (byte_len != 0) {

        //Read a byte to print
        uint8_t read = (uint8_t) *start_addr;
        
        if(read > 15) serial_printf(port, "0x{x.u32}    ", read);   //If it has two characters to print its ok
        else serial_printf(port, "0x{x.u32}0    ", read);           //otherwise we add a 0 to make it prettier
        
        byte_len--;     //One byte less to print
        start_addr++;   //Print next byte
        byte_number += 1;
        buffer[byte_number] = read;

        //If we displayed BYTES_PER_LINE bytes on this line, go down one line
        //or if we are displaying the last line
        if(byte_number == BYTES_PER_LINE || byte_len == 0) {
            
            //Print ASCII interpretation of the current line
            serial_printf(port, "   |");

            //Print and empty the buffer
            for(unsigned int i = 0; i < BYTES_PER_LINE; i++){
                serial_printf(port, "{c} ", buffer[i]);
                buffer[i] = 0;
            }

            serial_printf(port, "\f\r");
            byte_number = 0;
        }
    }
}