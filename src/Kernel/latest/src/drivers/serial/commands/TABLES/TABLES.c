/*
    Definition of TABLES command
*/

#include <stdint.h>
#include "tools/EBDA/EBDA.h"
#include "tools/RSDP/RSDP.h"
#include "tools/RSDT/RSDT.h"
#include "tools/FADT/FADT.h"
#include "drivers/serial/serial.h"

void cmd_TABLES(uint16_t port) {
/*
    Shows most important info about all found tables (currently related to ACPI)
*/

    serial_printf(port, "EBDA: 0x{x}\n", EBDA);
	serial_printf(port, "RSDP: 0x{x.u32}\n -> ACPI revision: {d}.0\n -> OEM: {s}\n",
                                RootSDP, RSDP_getACPIRevision(), RSDP_getOEMID());
	serial_printf(port, "RSDT: 0x{x.u32}\n", RootSDT);
	serial_printf(port, "FADT: 0x{x.u32}\n -> FADT revision: {d}.{d}\n -> 8042 present: {d}\n",
                    FixedADT, FixedADT->h.Revision, FixedADT->MinorRevision, FixedADT->BootArchitectureFlags & 0x02);
}