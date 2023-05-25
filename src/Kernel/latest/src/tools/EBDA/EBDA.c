/*
    Functions related to the Extended Bios Data Area
*/

#include <stdint.h>

//Bootloader has put EBDA's address at this address. It's often here even by default
#define EBDA_PTR_ADDRESS 0x040e

char* EBDA_getAddress() {
/*
    Returns a pointer to the address of the EBDA.

    Will return -1 on failure.
*/

    //Perform a 16 bit read of where the address of the EBDA is stored
    uint16_t* ptr = (uint16_t*) EBDA_PTR_ADDRESS;
    uint16_t address = *ptr;
    
    //Shift the address 4 bits to the left to get the true address
    char* realAddress = (char*)((uint32_t) address << 4);

    //If the EBDA is not in that area it is a big indication that something is wrong
    if (realAddress >= (char*)0x80000 && realAddress <= (char*)0x9ffff) return realAddress;
    else return (char*)-1;
}