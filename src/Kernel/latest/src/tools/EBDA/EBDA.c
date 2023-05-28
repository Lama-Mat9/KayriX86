/*
    Functions related to the Extended Bios Data Area
*/

#include <stdint.h>

//Bootloader has put EBDA's address at this address. It's often here even by default.
#define EBDA_PTR_ADDRESS 0x040e

//Available everywhere
char* EBDA;

int EBDA_init() {
/*
    Fills the global EBDA pointer with EBDA's address.

    Returns -1 on failure. Returns 0 on success.
*/

    //Perform a 16 bit read of where the address of the EBDA is stored
    uint16_t* ptr = (uint16_t*) EBDA_PTR_ADDRESS;
    uint16_t address = *ptr;
    
    //Shift the address 4 bits to the left to get the true address
    char* realAddress = (char*)((uint32_t) address << 4);

    //If the EBDA is not in that area it is a big indication that something is wrong
    if (realAddress < (char*)0x80000 || realAddress > (char*)0x9ffff){
        EBDA = 0;
        return -1;
    } 
    
    //Set the global EBDA pointer to EBDA's address
    EBDA = realAddress;
    
    return 0;
}