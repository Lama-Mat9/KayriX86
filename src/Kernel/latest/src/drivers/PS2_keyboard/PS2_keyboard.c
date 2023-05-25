/*
    PS2 keyboard driver for Intel 8042 controller (French AZERTY)

    A BIOS with "USB Legacy Support" will emumate this chip to let this driver communicate with USB keyboards.
*/

#include <stdint.h>
#include "drivers/PS2_keyboard/PS2_keyboard.h"
#include "microclib/port_io.h"

#define DATA_PORT 0x60          //R/W
#define STATUS_REGISTER 0x64    //R
#define COMMAND_REGISTER 0x64   //W

uint8_t PS2kb_readScancode() {
/*
    Reads and returns a scancode waiting in the 8042's buffer. 
*/

    //Need to check if buffer not empty before reading it
    if (PS2kb_readStatusBit(0) == 1)                //Buffer not empty
        return portIO_byte_read(DATA_PORT);
    else                                            //Buffer empty
        return 0;
}

uint8_t PS2kb_readStatusBit(int bitNumber) {
/*
    Returns 1 if the bit at bitNumber position in the status register is 1, 0 if it is 0.
    Bits go from 0 (included) to 7 (included).
*/
    return portIO_byte_read(STATUS_REGISTER) & (1 << bitNumber);
}