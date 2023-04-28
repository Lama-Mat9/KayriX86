/* Check if the compiler thinks you are targeting linux. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#include "drivers/screen/vga.h"
#include "drivers/serial/serial.h"

void kernel_main() {
	//Clear the screen before printing anything
	screen_clear();

	//	---- Welcome msg ----
	char welcomeString[] = { '[', 0x07, 'K', 0x07, 'a', 0x07, 'y', 0x07, 'r', 0x07, 'i', 0x07, 'X', 0x07,
 				'8', 0x07, '6', 0x07, ' ', 0x07, 'K', 0x09, 'e', 0x09, 'r', 0x09, 'n', 0x09,
				'e', 0x09, 'l', 0x09, ']', 0x07, '\n'};

	print_at_color(welcomeString, -1, -1);

	//Our driver will try to initialise multiple COM ports. Print how many were successfully initialised.
	print("Initialised serial ports: ");
	print_int(serial_init(), 10);
	print("\n");
}
