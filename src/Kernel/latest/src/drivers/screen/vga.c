/*
	VGA color compatible screen driver implementation.
*/

#include "vga.h" 				//For colors and things
#include "microclib/port_io.h"
#include "microclib/memcpy.h"

/*
	Few patterns to note before reading the print functions:
		- I've called a "char" a character without it's attribute byte
		- I've called a "cchar" a character with it's attribute byte (colored char).
*/

//	---------------------------------------------------------------------------------------------------

//Not exporting these functions outside of this file.
//They could but shouldn't be used anywhere else if possible.

uint8_t form_attribute_byte(uint8_t bg_color, uint8_t fg_color);
void get_screen_coordinates(int offset, int coordinates[2]);
void screen_scroll();
void cursor_formFeed();
void cursor_carriageReturn();
void cursor_newLine();

int get_cursor_offset();
int get_screen_offset(int row, int column);
void set_cursor_offset(int offset);

//	---------------------------------------------------------------------------------------------------

void VGA_print_cchar_at(char character, uint8_t fg_color, uint8_t bg_color, int row, int column) {
/*
	Function that prints one ASCII character using the GPU's tty mode.
	Providing a color for the background and foreground is required.
	Codes for these are in the .h file.
	Providing a valid row and column is also required.
*/
	//Security checks to ensure that we are writing inside of the screen
	if(row >= MAX_ROWS || column >= MAX_COLUMNS || row < 0 || column < 0) return;
	
	//Convert the bg and fg colors to usable attribute byte
	char attribute_byte = form_attribute_byte(bg_color, fg_color);

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

	//Convert the row / column coordinates to a memory offset for writing
	int offset = get_screen_offset(row, column);

	//In the GPU's tty mode we can simply write ASCII chars to video memory followed by their attributes.
	video_memory[offset] = character;
	video_memory[offset + 1] = attribute_byte;

	//If we have just printed a character at the last column of the last row.
	if(row == MAX_ROWS - 1 && column == MAX_COLUMNS - 1) {
		cursor_newLine();	//We scroll one line
	}
	else {
		//Otherwise we move the cursor 2 bytes (1 character).
		offset += 2;
		set_cursor_offset(offset);
	}
}

void VGA_print_cchar(char character, uint8_t fg_color, uint8_t bg_color) {
/*
	Wrapper for VGA_print_cchar_at()
	Writes the colored character at current cursor position.
*/

	//	---- Getting cursor coords ----
	int coordinates[2] = {0};	// [column, row]
	get_screen_coordinates(get_cursor_offset(), coordinates);

	//	---- Wrapping function ----
	VGA_print_cchar_at(character, fg_color, bg_color, coordinates[1], coordinates[0]);

}

void VGA_print_char(char character) {
/*
	Wrapper for VGA_print_cchar()
	Writes the character with default color at cursor position.
*/

	//	---- Wrapping function ----
	VGA_print_cchar(character, VGA_COLOR_DEFAULT, VGA_COLOR_BLACK);

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

	//Create a column / row buffer to fill with cursor coordinates
	int cursor_coordinates[2] = {0};
	int cursor_offset = get_cursor_offset();
	
	//Fill this buffer with the cursor's column / row coordinates
	get_screen_coordinates(cursor_offset, cursor_coordinates);
	
	//If we are on the last row, about to enter a row out of the screen, scrolling 1 row is enough.
	if (cursor_coordinates[1] == MAX_ROWS - 1) screen_scroll();
	else set_cursor_offset(cursor_offset + MAX_COLUMNS * 2);
	//Adding twice the amount of characters of one line works.
	
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
	Function that converts a byte offset relative to the video address into column / row coordinates.
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

uint8_t form_attribute_byte(uint8_t bg_color, uint8_t fg_color) {
/*
	Function that takes in a background and foreground color,
	and returns the attribute byte for these colors.
*/
	return bg_color << 4 | fg_color;
}