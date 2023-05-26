/* Check if the compiler thinks you are targeting linux. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#include "drivers/screen/vga.h"
#include "drivers/serial/serial.h"
#include "drivers/PIC/pic.h"
#include "microclib/stdio/printf.h"
#include "tools/EBDA/EBDA.h"
#include "tools/RSDP/RSDP.h"
#include "tools/RSDT/RSDT.h"
#include "tools/FADT/FADT.h"

//Function defined and exported in assembly.
extern void idt_load(void);

void kernel_main() {

	//	---- Welcome msg ----
	screen_clear();
	printf("[KayriX86 Kernel]\n");

	//	---- Setting up interrupts  ----
	//Initialises the master PIC to use INTs 32 -> 39,
	//		and the slave PIC to use INTs 40 -> 47.
	//IRQs still disabled for now though.
	PIC_init(0x20, 0x28);

	//Enables protected mode interrupts.
	idt_load();

	//Enable PS2 keyboard interrupts
	PIC_IRQ_enable(1);

	//	---- Important drivers initialisation ----
	//Our driver will try to initialise multiple COM ports. 
	//Initialise and print how many were successfully initialised.
	printf("Initialised serial ports: {d}\n", serial_init());

	// ---- Memory locations detection ----
	EBDA_init();	//Fill the EBDA struct
	RSDP_init();	//Fill the RSDP struct
	RSDT_init();	//Fill the RSDT struct
	FADT_init();	//Fill the FADT struct

	printf("EBDA: 0x{x}\n", EBDA);
	printf("RSDP: 0x{x}\n -> ACPI revision: {d}.0\n -> OEM: {s}\n", RootSDP, RSDP_getACPIRevision(), RSDP_getOEMID());
	printf("RSDT: 0x{x}\n", RootSDT);
	printf("FADT: {x}\n", FixedADT);
}
