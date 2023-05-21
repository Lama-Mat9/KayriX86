/*
	VGA color compatible screen driver implementation.
*/

#include "vga.h" //For colors and things
#include "../../microclib/std.h"
#include "../../microclib/port_io.h"
#include "../../microclib/memcpy.h"

/*
	Few patterns to note before reading the print functions:
	Color attributes of 0x00 will be interpreted as default print color.
	Negative row / column values will make the string be printed at current cursor position.
*/

uint8_t form_attribute_byte(uint8_t bg_color, uint8_t fg_color) {
/*
	Function that takes in a background and foreground color,
	and returns the attribute byte for these colors.
*/
	return bg_color << 4 | fg_color;
}

void print_int(int32_t value, int base) {
/*
	Function that prints an integer of any base.
*/

	//To determine the amount of space we need to store the number we
	//have to determine how many digits are in the number.
	int32_t valuecpy = value;
	int digits = 0;
	while (valuecpy != 0) {
		valuecpy /= base;
		digits += 1;
	}

	//If the value is negative we need another bye for '-'
	if(value < 0) digits++;

	//Allocate one byte more than the digits for \0
	char buffer[digits + 1];

	itoa(value, buffer, base);

	print(buffer);
}

void print(char* string) {
/*
	Function that prints a string using the default color at the current cursor position.
*/
	print_at(string, -1, -1);
}

void print_at(char* string, int row, int column) {
/*
	Function that prints a string at the specified location using the default color.
*/

	//Keeps track of the character to be printed.
        char* nextChar = string;

        while(*nextChar != '\0') {
                print_char(*nextChar, 0, row, column + nextChar - string);
                nextChar++;  //Move forward 1 character.
        }

}

void print_at_color(char* colored_string, int row, int column) {
/*
	Function that prints a "colored string" at a specified location.
	The "colored string" must be a string where each character is followed by it's attribute byte.

	Example of "colored string":
	char test[] = {'t', 0x0e, 'e', 0x0f, 'a', 0x0a};	//You can see each character and it's attribute
	char test2[] = "o\x0e";		//This one is an 'o' with 0x0e attribute
*/

	//Keeps track of the character to be printed.
	char* nextChar = colored_string;

	while(*nextChar != '\0') {
		print_char(nextChar[0], nextChar[1], row, column + nextChar - colored_string);
		nextChar += 2;	//Move forward 1 character and 1 attribute byte so 2 bytes.
	}
}

void print_char(char character, char attribute_byte, int row, int column) {
/*
	Function that prints one ASCII character using the GPU's tty mode.
	You can optionally provide:
		attribute_byte:		Standardized attributes used by GPUs in tty mode. (Default white on black).
		column / row:		Specify at which position on the screen the character should be printed
										(Default at cursor position)
*/

	//		---- Special behaviors definition ----
	switch (character) {
		case '\n':	//The new line character
			cursor_newLine();
			return;	//Dont continue the function else it's gonna print the special char as ASCII
		case '\f':	//The form feed character
			cursor_formFeed();
			return;
		case '\r':	//The carriage return character
			cursor_carriageReturn();
			return;
	}

	//We keep the start of video memory in a 1 byte pointer.
	unsigned char* video_memory = (unsigned char*) VIDEO_ADDRESS;

	//Default print color
	if(!attribute_byte) attribute_byte = VGA_COLOR_DEFAULT;

	//In case that we weren't provided coordinates, we retrieve the cursor's row and column to use these instead.
	if(row < 0 || column < 0) {
		int coordinates[2];
		get_screen_coordinates(get_cursor_offset(), coordinates);
		column = coordinates[0];	//X coordinates are the column
		row = coordinates[1];		//Y coordinates are the row
	}

	//Then convert back the row / column coordinates to a memory offset
	int offset = get_screen_offset(row, column);

	//In the GPU's tty mode we can simply write ASCII chars to video memory followed by their attributes.
	video_memory[offset] = character;
	video_memory[offset + 1] = attribute_byte;

	//If we have just printed a character at the last column of the last row.
	if(row == MAX_ROWS - 1 && column == MAX_COLUMNS - 1) {
		screen_scroll();	//We scroll one line
	}
	else {
		//Else we move the cursor 2 bytes (1 character).
		offset += 2;
		set_cursor_offset(offset);
	}
}

void screen_scroll() {
/*
	Function that moves up 1 row all of the rows of the GPU's tty.
	This is useful when the cursor reaches the end of the screen for example.
*/
	//We need to copy all rows except the first one. We will copy these up 1 row.
	memcpy((char*) VIDEO_ADDRESS + get_screen_offset(1, 0), (char*) VIDEO_ADDRESS, (MAX_ROWS-1) * 2 * MAX_COLUMNS);
		//From end of first row in memory	To first row	Copying all of the screen minus one row.

	//Once we've done that we need to cleanup the last row so that it can be used.
	char* last_row = (char*) VIDEO_ADDRESS + get_screen_offset(MAX_ROWS - 1, 0);	//Get the address of the last row.

	//Fill the whole row with nothingness.
	for(int i = 0; i < MAX_COLUMNS * 2; i += 2) {
		last_row[i] = 0;
		last_row[i + 1] = VGA_COLOR_DEFAULT;	//Fill the attribute byte of each character with the default value
	}						//as the cursor will use that value for it's color when it's there.

	//Then we need to put the cursor back at the start of the same row.
	cursor_carriageReturn();
}

void screen_clear() {
/*
	Clears the screen with default color attributes and sets cursor at start position.
*/
	//Holds the pointer to the byte we are going to erase. Starts at the start of the GPU's tty video memory.
	char* video_memory = (char*) VIDEO_ADDRESS;

	//Holds the last byte that we want to erase.
	char* lastByte = (char*) VIDEO_ADDRESS + get_screen_offset(MAX_ROWS - 1, MAX_COLUMNS - 1);

	//While the current byte is not the last one
	while(video_memory <= lastByte) {
		video_memory[0] = 0;			//Erase the character byte
		video_memory[1] = VGA_COLOR_DEFAULT;	//Reset the attribute byte
		video_memory += 2;			//Move forward over the bytes we've just erased.
	}

	//Reset the cursor position.
	set_cursor_offset(0);
}

void cursor_formFeed() {
/*
	Function that moves the cursor to it's current X position but on the next line.
*/
	//Simply adding twice the amount of characters of one line will do the job
	set_cursor_offset(get_cursor_offset() + MAX_COLUMNS * 2);
}

void cursor_carriageReturn() {
/*
	Function that moves the cursor at the start of the line that it's already on.
*/
	int offset = get_cursor_offset();

	//We calculate how many characters there are from the start of the current line,
	//and substract them from the offset address.
	set_cursor_offset(offset - offset % (MAX_COLUMNS * 2));
}

void cursor_newLine() {
/*
	Function that moves the cursor at the beginning of the next line.
*/
	cursor_formFeed();
	cursor_carriageReturn();
}

int get_cursor_offset() {
/*
	Function that returns the memory offset of the current cursor position
	made specifically for the GPU's tty mode.
	The offset is relative to the start of the video mapped tty memory start address.

	How it works:
	We communicate (read) some VGA registers using their port addresses.
	Port 0x3D4 (Control register) specifies an index value for what we need to access in port 0x3D5.
	Port 0x3D5 (Data register) at index 0x0E contains the upper half (1 byte) of the (2 byte) cursor position.
	Port 0x3D5 (Data register) at index 0x0F contains the lower half (1 byte) of the (2 byte) cursor position.

		---- Ressources ----
	https://bochs.sourceforge.io/techspec/PORTS.LST
	Note: The following information could be inaccurate.

	Color Graphics Adapter (CGA) port IO specs:

	0x03D4: Can be written to (0-11h) as a way to select the index you want in 0x03D5
	0x03D5:
		0x0E (rw): Cursor location high byte		//The cursor location seems to be stored in two bytes. 
		0x0F (rw): Cursor location low byte		//Need to read both to get real location.
*/
	int offset = 0;

	//Specify as index value that we want to access the higher byte of the cursor's location.
	portIO_byte_write(CGA_REGISTER_CTRL, 0x0E);

	//Read the higher byte of the cursor's location and store it as the lower byte or our variable.
	offset = portIO_byte_read(CGA_REGISTER_DATA);

	//Now set the higher byte back to where it should be by shifting it 1 byte to the left.
	offset = offset << 8;

	//Specify as index value that we want to access the lower byte of the cursor's location.
	portIO_byte_write(CGA_REGISTER_CTRL, 0x0F);

	//Read the lower byte of the cursor's location and OR it with our variable so that it forms the full word.
	offset = offset | portIO_byte_read(CGA_REGISTER_DATA);

	//The VGA hardware returns the offset in number of characters from the start, not in number of bytes.
	//This means that we multiply the offset to take into account each attribute byte.
	return offset * 2;
}

void set_cursor_offset(int offset) {
/*
	Function that modifies the memory offset of the cursor in the GPU's tty mode.
	May be used to change the cursor position.

	How it works:
	We write to some VGA registers using their port addresses.
	Port 0x3D4 (Control register) specifies an index value for what we need to access in port 0x3D5.
        Port 0x3D5 (Data register) at index 0x0E contains the upper half (1 byte) of the (2 byte) cursor position.
        Port 0x3D5 (Data register) at index 0x0F contains the lower half (1 byte) of the (2 byte) cursor position.

	It works pretty much the same as it's get counterpart.
	You may find useful information by reading it.
*/

	//The VGA hardware uses offsets in number of characters, not in bytes.
	//The offset we are given is in bytes, therefore we have to divide it by 2
	//because a single character is two bytes in the GPU's tty mode (character byte, attribute byte).
	offset /= 2;

	//By ANDing it with 0000000011111111b we keep lower byte.
	unsigned char lower_byte = offset & 0xFF;

	//By ANDing it with 1111111100000000b and shifting high byte to low byte we keep the higher byte.
	unsigned char higher_byte = (offset & 0xFF00) >> 8;

	//Specify as index value that we want to access the higher byte of the cursor's location.
        portIO_byte_write(CGA_REGISTER_CTRL, 0x0E);

	//Set the higher byte of the cursor position to the offset's higher byte.
	portIO_byte_write(CGA_REGISTER_DATA, higher_byte);

	//Specify as index value that we want to access the lower byte of the cursor's location.
        portIO_byte_write(CGA_REGISTER_CTRL, 0x0F);

	//Set the lower byte of the cursor position to the offset's lower byte.
        portIO_byte_write(CGA_REGISTER_DATA, lower_byte);
}

int get_screen_offset(int row, int column) {
/*
	Function that converts a row/column value into a memory offset
	made specifically for the GPU's tty mode.
	The offset is relative to the start of the video mapped tty memory start address.

	How it works:
	In GPU tty mode, two bytes make a single character (character byte, attribute byte).
	This means that to move down 1 row we need to move 2 times the row length.
	Multiply all of that by the amount of rows we want to go down and you are done for the rows.

	For the columns, simply multiply the amount of columns we want to skip by two and you are done.
	-> offset = 2 * row * MAX_COLUMNS + 2 * column
*/
	//Read the explanation to know how i found this.
	int offset = 2 * row * MAX_COLUMNS + 2 * column;

	return offset;
}

void get_screen_coordinates(int offset, int coordinates[2]) {
/*
	Function that converts a byte offset relative to the video address into row / column coordinates.
	This of course is made for the GPU's tty mode.

	The coordinates parameter is there because we need to return two values (X and Y)
	and returning a pointer to a local array won't work (destroyed).
	So the function returns the coordinates in the memory areas you give it in the coordinates parameter.
*/
	offset /= 2; //Converting 2 bytes to 1 char by dividing the result by two

	//The X coordinate of the offset address is what's left of the offset after it's at the correct row.
	coordinates[0] = offset % MAX_COLUMNS;

	coordinates[1] = (offset - coordinates[0]) / MAX_COLUMNS;
}
