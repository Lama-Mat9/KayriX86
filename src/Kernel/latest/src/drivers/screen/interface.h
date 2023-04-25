/*
	This file defines settings for the screen driver.
*/

//This is the default memory location where the GPU intercepts incoming data
//and interprets it as ASCII then writes it to the display.
#define VIDEO_ADDRESS 0xb8000

#define MAX_ROWS 25	//These won't actually change the display mode.
#define MAX_COLUMNS 80	//Our bootsector program uses a bios routine to set this display mode by default.
			//Our driver doesn't fetch these previously set values so they are hardcoded here :c

//The GPU reserved tty memory location we talked about accepts an "Attribute byte".
#define WHITE_ON_BLACK 0x07	//This is the specified value for white on black. DYOR for more codes info

//Color Graphics Adapter (CGA) port addresses (https://bochs.sourceforge.io/techspec/PORTS.LST)
#define CGA_REGISTER_CTRL 0x03D4		//Register used to keep the index of requested data in CGA_REGISTER_DATA
#define CGA_REGISTER_DATA 0x03D5		//Indexed r/w (some not writable) register containing CGA information
