/* Check if the compiler thinks you are targeting linux. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#include "drivers/screen/vga.h"
#include "drivers/serial/serial.h"
#include "drivers/PIC/pic.h"
#include "microclib/stdio/printf.h"

//Function defined and exported in assembly.
extern void idt_load(void);

void kernel_main() {

	//Clear the screen before printing anything
	screen_clear();

	//Initialises the master PIC to use INTs 32 -> 39,
	//		and the slave PIC to use INTs 40 -> 47.
	//IRQs still disabled for now though.
	PIC_init(0x20, 0x28);

	//Enables protected mode interrupts.
	idt_load();

	//	---- Welcome msg ----
	printf("[KayriX86 Kernel]\n");

	//Our driver will try to initialise multiple COM ports. 
	//Initialise and print how many were successfully initialised.
	printf("Initialised serial ports: {d}\n", serial_init());
}
