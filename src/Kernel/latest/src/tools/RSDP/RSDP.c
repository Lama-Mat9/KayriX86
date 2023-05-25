/*
    Functions related to the RSDP
*/

#include <stdint.h>
#include "tools/EBDA/EBDA.h"
#include "microclib/memcmp.h"
#include "microclib/memcpy.h"

//This list of functions is incomplete, as the RSDP contains more fields, but we do not need the rest for now.

char* RSDP_getAddress() {
/*
    Tries to find the address of the RSDP, then returns it.

    Returns -1 if not found
*/

    //This is exactly like the string we have to find
    char* rsd_ptr = "RSD PTR ";

    //First try: Finding the string "RSD PTR " inside the first 1kb of the EBDA
    char* ebda_ptr = EBDA_getAddress();
    char* limit = ebda_ptr + 1000;

    for (size_t i = 0; &(ebda_ptr[i]) < limit; i += 8) {
        if(memcmp(&(ebda_ptr[i]), rsd_ptr, 8) == 0)
            return &(ebda_ptr[i]);
    }

    //Second try: Between 0x000E0000 and 0x000FFFFF (the main BIOS area below 1 MB)
    char* start = (char*) 0x000E0000;
    limit = (char*) 0x000FFFFF;

    for (size_t i = 0; &(start[i]) < limit; i += 8) {
        if(memcmp(&(start[i]), rsd_ptr, 8) == 0)
            return &(start[i]);
    }
    
    return (char*) -1;
}

uint8_t RSDP_getACPIRevision() {
/*
    Returns an approximate ACPI revision number.
    
    Returns 1 if ACPI revision 1.0
    Returns 2 if ACPI revision 2.0 to 6.1

    Returns -1 on failure
*/

    //Need start address of rsdp structure
    char* rsdp_addr = RSDP_getAddress();

    //Error handling
    if((int) rsdp_addr == -1) return -1;

    //Revision is always 7 bytes after base address no matter the version
    uint8_t revision = *(rsdp_addr + 7);

    //Translate the codes to corresponding versions
    if (revision == 0x20) return 2;
    else if (revision == 0) return 1;
    
    //Error handling
    return -1;
}
char* RSDP_getOEMID() {
/*
    Returns the OEMID field of the RSDP structure, with a null terminator.

    Returns -1 on failure.
    Returns the buffer address on success.
*/

    //Need start address of rsdp structure
    char* rsdp_addr = RSDP_getAddress();

    //Error handling
    if((int) rsdp_addr == -1) return (char*) -1;

    //OEMID is always 9 bytes after base address no matter the version, and always 6 bytes long.
    rsdp_addr += 9;
    static char OEMID_addr[7];
    
    //Setting the null terminator
    OEMID_addr[7] = 0;

    memcpy(rsdp_addr, OEMID_addr, 6);

    return OEMID_addr;
}