/*
    Functions related to the RSDP
*/

#include <stdint.h>
#include "tools/EBDA/EBDA.h"
#include "tools/RSDP/RSDP.h"
#include "microclib/memcmp.h"
#include "microclib/memcpy.h"

//Available everywhere.
struct RSDP* RootSDP;

//Local prototypes
int RSDP_isValid(struct RSDP* RSDP_addr);

int RSDP_init() {
/*
    Fills the global RSDP structure with the RSDP.

    Returns -1 on error. Returns 0 if ok.

    Requires a valid pointer to the EBDA.
*/

    //This is exactly like the string we have to find
    char* rsd_ptr = "RSD PTR ";

    //First try: Finding the string "RSD PTR " inside the first 1kb of the EBDA
    char* ebda_ptr = EBDA;
    char* limit = (ebda_ptr + 1000);

    for (size_t i = 0; &(ebda_ptr[i]) < limit; i += 8) {    //Check each 8 bytes to see if they correspond to string
        if(memcmp(&(ebda_ptr[i]), rsd_ptr, 8) == 0 && RSDP_isValid( (struct RSDP*) &(ebda_ptr[i]) )) {
            RootSDP = (struct RSDP*) &(ebda_ptr[i]);     // Set the struct pointer to the RSDP's address
            return 0;
        }
            
    }

    //Second try: Between 0x000E0000 and 0x000FFFFF (the main BIOS area below 1 MB)
    char* start = (char*) 0x000E0000;
    limit = (char*) 0x000FFFFF;

    for (size_t i = 0; &(start[i]) < limit; i += 8) {   //Check each 8 bytes to see if they correspond to string
        if(memcmp(&(start[i]), rsd_ptr, 8) == 0 && RSDP_isValid( (struct RSDP*) &(start[i]))) {
            RootSDP = (struct RSDP*) &(start[i]);    // Set the struct pointer to the RSDP's address
            return 0;
        }
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
    if (RootSDP->Revision == 2) return 2;
    else if (RootSDP->Revision == 0) return 1;

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
    memcpy(RootSDP->OEMID, OEMID_addr, 6);

    return OEMID_addr;
}

int RSDP_isValid(struct RSDP* RSDP_addr) {
/*
    Checks if the provided RSDP address is valid.

    Returns 1 if the table is valid, 0 otherwise.
*/
    
    //Add all the bytes of the ACPI 1.0 RSDP structure
    uint32_t sum = 0;
    for (int i = 0; i < 20; i++) {
        sum += (uint32_t) *( (char*) RSDP_addr + i );
    }

    //If the result's lowest byte is all zeros, RSDP (ACPI V1.0) is valid.
    //Anything else and it isnt.
    int acpiv1_isValid =  (sum & 255) == 0;

    //If ACPI version < 2.0 that's all we need to check
    if (RSDP_addr->Revision == 0) return acpiv1_isValid;
    else if (acpiv1_isValid) {  //If ACPI version >= 2.0, we need to check the rest as well.

        //Add all the bytes of the ACPI >= 2.0 RSDP structure
        sum = 0;
        for (int i = 20; i < 36; i++)
            sum += (uint32_t) *( (char*) RSDP_addr + i );

        //If the result''s lowest byte is all zeos, RSDP (ACPI V2.0) is valid.
        //Anything else and it isnt.
        return (sum & 255) == 0;
    }       
    

    return 0;
}