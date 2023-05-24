/* Check if the compiler thinks you are targeting linux. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#include "drivers/screen/vga.h"
#include "drivers/serial/serial.h"
#include "drivers/PIC/pic.h"
#include "microclib/sprintf.h"

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
	char welcomeString[] = { '[', 0x07, 'K', 0x07, 'a', 0x07, 'y', 0x07, 'r', 0x07, 'i', 0x07, 'X', 0x07,
 				'8', 0x07, '6', 0x07, ' ', 0x07, 'K', 0x09, 'e', 0x09, 'r', 0x09, 'n', 0x09,
				'e', 0x09, 'l', 0x09, ']', 0x07, '\n'};

	print_at_color(welcomeString, -1, -1);

	//Our driver will try to initialise multiple COM ports. Print how many were successfully initialised.
	print("Initialised serial ports: ");
	print_int(serial_init(), 10);
	print("\n");

	/*char buf[100];
	sprintf(buf, "This is sprintf {{c}}", 'a');
	print(buf);*/
}
