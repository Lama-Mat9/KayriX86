/*
    Definition of PIC command

*/

#include <stdint.h>
#include <drivers/PIC/pic.h>
#include <drivers/serial/serial.h>

//Local prototypes
static int count_set_bits(uint16_t bits);

void cmd_PIC(uint16_t port) {
/*
    Lists which IRQs the PIC is waiting for the CPU to finish servicing, and also
    which are waiting to be serviced.
*/

    // ---- Bitmask ----
    
    serial_printf(port, "Current bitmask: {b.u32}\f\r", PIC_getBitmask());

    // ---- IRR ----

    //Get a bitmask of all pending IRQs
    uint16_t IRR = PIC_ReadIRR();

    serial_printf(port, "Pending IRQs: {d}\f\r", count_set_bits(IRR));

    for (unsigned int i = 0; i < 16; i++) {
        uint16_t mask = 1 << i;     //Set bit at position i and unset all the others

        //If bit is set we print the interrupt number
        if (IRR & mask) serial_printf(port, " -> IRQ {d}\f\r", i);
    }


    // ---- ISR ----

    //Get a bitmask of all pending IRQs
    uint16_t ISR = PIC_ReadISR();

    serial_printf(port, "IRQs awaiting EOI: {d}\f\r", count_set_bits(ISR));

    for (unsigned int i = 0; i < 16; i++) {
        uint16_t mask = 1 << i;     //Set bit at position i and unset all the others

        //If bit is set we print the interrupt number
        if (ISR & mask) serial_printf(port, " -> IRQ {d}\f\r", i);
    }
    
}

static int count_set_bits(uint16_t bits) {
/*
    Returns how many bits are set to 1 in a binary number.
*/

    //Counts the result
    unsigned int count = 0;

    while (bits > 0) {
        count += bits & 1;      //Add the value of least significant bit to count
        bits >>= 1;         //Remove this bit by shifting 1 bit ot the right
    }

    return count;
}