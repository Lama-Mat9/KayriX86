#include "interface.h"

/*
	VGA color compatible screen driver implementation.
*/

//		---- Internal function prototypes ----
void print_char(char character, char attribute_byte, int row, int column);
int get_cursor_offset();
int get_screen_offset(int row, int column);
void set_cursor_offset(int offset);

//		---- External function prototypes ----
unsigned char portIO_byte_read(unsigned short int port);
void portIO_byte_write(unsigned short int port, unsigned char byte);

void print_char(char character, char attribute_byte, int row, int column) {
/*
	Function that prints one ASCII character using the GPU's tty mode.
	You can optionally provide:
		attribute_byte:		Standardized attributes used by GPUs in tty mode. (Default white on black).
		column / row:		Specify at which position on the screen the character should be printed
										(Default at cursor position)

	If you need to print at current cursor position, set row and/or column to negative values.
	If you don't want to give attributes, it to 0.
*/
	//We keep the start of video memory in a 1 byte pointer.
	unsigned char* video_memory = (unsigned char*) VIDEO_ADDRESS;

	//Default print color
	if(!attribute_byte) attribute_byte = WHITE_ON_BLACK;

	//Contains the memory offset of where we are going to put the character, relative
	//to the start of the video mapped tty memory start address.
	int offset;
	if(row >= 0 && row <= MAX_ROWS && column >= 0 && column <= MAX_COLUMNS) {	//If row and column valid
		offset = get_screen_offset(row, column);		//We use them for offset
	}
	else {
		offset = get_cursor_offset();		//We will print at the current cursor position
	}

	//TODO: Newline support
	//	Text scrolling support
	//	Cursor follows printed character support

	//In the GPU's tty mode we can simply write ASCII chars to video memory followed by their attributes.
	video_memory[offset] = character;
	video_memory[offset + 1] = attribute_byte;

	//We have printed one character. We can now move the cursor 2 bytes (1 character).
	offset += 2;
	set_cursor_offset(offset);
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
	offset << 8;

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
	portIO_byte_write(CGA_REGISTER_CTRL, higher_byte);

	//Specify as index value that we want to access the lower byte of the cursor's location.
        portIO_byte_write(CGA_REGISTER_CTRL, 0x0F);

	//Set the lower byte of the cursor position to the offset's lower byte.
        portIO_byte_write(CGA_REGISTER_CTRL, lower_byte);
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
