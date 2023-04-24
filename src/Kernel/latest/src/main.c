
//		---- External function prototypes ----
void print_char(char character, char attribute_byte, int row, int column);

void main() {
	char string[4] = "owo";
	char* readchar = string;

	while(*readchar != '\0') {
		print_char(*readchar, 0, -1, -1);
		readchar++;
	}
}
