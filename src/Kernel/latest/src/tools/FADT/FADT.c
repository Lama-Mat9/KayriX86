/*
    Functions related to the FADT
*/

#include "tools/FADT/FADT.h"
#include "tools/RSDT/RSDT.h"
#include "tools/ACPI/SDTHeader.h"
#include "microclib/memcmp.h"

struct FADT* FixedADT;

int FADT_init() {
/*
    Fills the global pointer to the FADT.

    Returns -1 on failure. Returns 0 on success.

    Requires a valid RSDT structure.
*/

    //Store how many entries there are in the RSDT
    int RSDT_entries = (RootSDT->SDTheader.Length - sizeof(RootSDT->SDTheader)) / 4;

    //Stores the address of the first SDT pointer entry
    uint32_t* entry_ptr = (uint32_t*) &(RootSDT->FirstSDT);

    for (int i = 0; i < RSDT_entries; i++) {

        //Take the entry i in the list of SDT entries
        struct ACPISDTHeader* SDTheader = (struct ACPISDTHeader*) entry_ptr[i];     
        
        //Compare the entry's signature with the FADT's signature to recognise if it is the FADT entry
        if(memcmp(SDTheader->Signature, "FACP", 4) == 0) {
            FixedADT = (struct FADT*) SDTheader;    //Fill the global struct
            return 0;
        }

    }
    
    return -1;
}