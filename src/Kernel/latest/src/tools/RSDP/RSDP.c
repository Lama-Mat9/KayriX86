/*
    Functions related to the RSDP
*/

#include <stdint.h>
#include "tools/EBDA/EBDA.h"
#include "tools/RSDP/RSDP.h"
#include "microclib/memcmp.h"
#include "microclib/memcpy.h"

//Available everywhere.
struct RSDPDescriptor* RSDP;

int RSDP_init() {
/*
    Fills the global structure with the RSDP.

    Returns -1 if not found
*/

    //This is exactly like the string we have to find
    char* rsd_ptr = "RSD PTR ";

    //First try: Finding the string "RSD PTR " inside the first 1kb of the EBDA
    char* ebda_ptr = EBDA_getAddress();
    char* limit = ebda_ptr + 1000;

    for (size_t i = 0; &(ebda_ptr[i]) < limit; i += 8) {
        if(memcmp(&(ebda_ptr[i]), rsd_ptr, 8) == 0)
            RSDP = (struct RSDPDescriptor*) &(ebda_ptr[i]);     // Set the struct pointer to the RSDP's address
    }

    //Second try: Between 0x000E0000 and 0x000FFFFF (the main BIOS area below 1 MB)
    char* start = (char*) 0x000E0000;
    limit = 0x000FFFFF;

    for (size_t i = 0; &(start[i]) < limit; i += 8) {
        if(memcmp(&(start[i]), rsd_ptr, 8) == 0)
            RSDP = (struct RSDPDescriptor*) &(start[i]);    // Set the struct pointer to the RSDP's address
    }
    
    //Not found
    return -1;
}

uint8_t RSDP_getACPIRevision() {
/*
    Returns an approximate ACPI revision number.
    
    Returns 1 if ACPI revision 1.0
    Returns 2 if ACPI revision 2.0 to 6.1

    Returns -1 on failure
*/

    //Translate the codes to corresponding versions
    if (RSDP->Revision == 2) return 2;
    else if (RSDP->Revision == 0) return 1;

    //Error handling
    return -1;
}

char* RSDP_getOEMID() {
/*
    Returns the OEMID field of the RSDP structure, with a null terminator.

    Returns -1 on failure.
    Returns the buffer address on success.
*/

    //OEMID is always 9 bytes after base address no matter the version, and always 6 bytes long.
    static char OEMID_addr[7];
    
    //Setting the null terminator
    OEMID_addr[7] = 0;

    //Getting the OEMID from the struct.
    memcpy(RSDP->OEMID, OEMID_addr, 6);

    return OEMID_addr;
}

int RSDP_isValid(struct RSDPDescriptor* RSDP_addr) {
/*
    Checks if the provided RSDP address is valid.

    Returns 0 if the table is ok, anything else if it isn't.
*/

    //20 bytes to add

    //If ACPI version is < 2
    if (RSDP_addr->Revision == 0) {
        
        //Add all the bytes of the RSDP structure
        uint32_t sum = 0;
        for (int i = 0; i < 20; i++) {
            sum += (int) ( *((char*) RSDP_addr + i));
        }
    
        return sum;
    }
    
    //If ACPI version is > 2

    return -1;
}