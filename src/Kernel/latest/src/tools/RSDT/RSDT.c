/*
    Functions related to the RSDT
*/

#include <stdint.h>
#include "microclib/memcmp.h"
#include "tools/RSDT/RSDT.h"
#include "tools/RSDP/RSDP.h"

//Available everywhere
struct RSDT* RootSDT;

//Local prototypes
int RSDT_isValid(struct RSDT* RSDT_addr);

int RSDT_init() {
/*
    Fills the global pointer to the RSDT.

    Returns -1 on failure. Returns 0 on success.

    Requires a valid RSDP structure.
*/

    if(RSDT_isValid( (struct RSDT*) RootSDP->RsdtAddress)) {
        RootSDT = (struct RSDT*) RootSDP->RsdtAddress;
        return 0;
    }

    return -1;
}

int RSDT_isValid(struct RSDT* RSDT_addr) {
/*
    Checks if the provided RSDT address is valid.

    Returns 1 if the address is valid, 0 otherwise.
*/

    char* signature = "RSDT";
    if (memcmp(RSDT_addr->SDTheader.Signature, signature, 4) == 0) return 1;
    
    return 0;
}