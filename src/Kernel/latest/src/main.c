
//		---- External function prototypes ----
void print(char* string);
void print_at_color(char* colored_string, int row, int column);

void screen_clear();

void main() {
	//Clear the screen before printing anything
	screen_clear();

	//	---- Welcome msg ----
	char welcomeString[] = { '[', 0x07, 'K', 0x07, 'a', 0x07, 'y', 0x07, 'r', 0x07, 'i', 0x07, 'X', 0x07,
 				'8', 0x07, '6', 0x07, ' ', 0x07, 'K', 0x09, 'e', 0x09, 'r', 0x09, 'n', 0x09,
				'e', 0x09, 'l', 0x09, ']', 0x07, '\n'};

	print_at_color(welcomeString, -1, -1);
}
