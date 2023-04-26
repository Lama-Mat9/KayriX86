/*
	This file defines settings for the screen driver.
*/

//Prevent double inclusions by using this technique
#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stddef.h>

//This is the default memory location where the GPU intercepts incoming data
//and interprets it as ASCII then writes it to the display.
#define VIDEO_ADDRESS 0xb8000

#define MAX_ROWS 25	//These won't actually change the display mode.
#define MAX_COLUMNS 80	//Our bootsector program uses a bios routine to set this display mode by default.
			//Our driver doesn't fetch these previously set values so they are hardcoded here :c

//Color Graphics Adapter (CGA) port addresses (https://bochs.sourceforge.io/techspec/PORTS.LST)
#define CGA_REGISTER_CTRL 0x03D4		//Register used to keep the index of requested data in CGA_REGISTER_DATA
#define CGA_REGISTER_DATA 0x03D5		//Indexed r/w (some not writable) register containing CGA information

/*
	Provides access to the standardised color codes for the attribute bytes
	of the VGA text array.
*/

#define	VGA_COLOR_BLACK 0
#define	VGA_COLOR_BLUE 1
#define	VGA_COLOR_GREEN 2
#define	VGA_COLOR_CYAN 3
#define	VGA_COLOR_RED 4
#define	VGA_COLOR_MAGENTA 5
#define	VGA_COLOR_BROWN 6
#define	VGA_COLOR_LIGHT_GREY 7
#define	VGA_COLOR_DARK_GREY 8
#define	VGA_COLOR_LIGHT_BLUE 9
#define	VGA_COLOR_LIGHT_GREEN 10
#define	VGA_COLOR_LIGHT_CYAN 11
#define	VGA_COLOR_LIGHT_RED 12
#define	VGA_COLOR_LIGHT_MAGENTA 13
#define	VGA_COLOR_LIGHT_BROWN 14
#define	VGA_COLOR_WHITE 15

#define VGA_COLOR_DEFAULT VGA_COLOR_LIGHT_GREY

//              ---- Internal function prototypes ----
void print_char(char character, char attribute_byte, int row, int column);
int get_cursor_offset();
int get_screen_offset(int row, int column);
void set_cursor_offset(int offset);
void print_at_color(char* colored_string, int row, int column);
void print_at(char* string, int row, int column);
void print(char* string);
void screen_scroll();
void get_screen_coordinates(int offset, int coordinates[2]);
void cursor_formFeed();
void cursor_carriageReturn();
void cursor_newLine();
void screen_clear();
uint8_t form_attribute_byte(uint8_t bg_color, uint8_t fg_color);
void print_int(int32_t value, int base);

//End the prevent double inclusions technique
#endif
