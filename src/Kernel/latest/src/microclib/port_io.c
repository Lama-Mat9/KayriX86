/*
	Provides handy functions so that C programs can interact with ASM port handling instructions
	For information about ports: https://wiki.osdev.org/I/O_Ports
*/

unsigned char portIO_byte_read(unsigned short int port) {
/*
	Function to read one byte from given port address and return it.
	input size: 2bytes
	output size: 1byte
*/
	unsigned char result;		//Allocate 1 byte for return value.
	__asm__("in %1, %0"		//GAS syntax. Use DX address as port address and put result in AL.
		: "=a" (result)		//Use AL value as output value and put it in result variable.
		: "d" (port)		//Use DX as input register. Set DX to port variable value.
		);			//No clobbered registers, GCC knows them as we told it which we use.

	return result;
}

void portIO_byte_write(unsigned short int port, unsigned char byte) {
/*
        Function to write one byte at given port address.
        input size: 2bytes, 1byte
*/
	__asm__("out %%al, %%dx"		//GAS syntax. Write AL to address in DX.
		:				//No output
		: "a" (byte), "d" (port)	//byte -> AL, port -> DX
		);				//No clobbered registers.
}

unsigned short int portIO_word_read(unsigned short int port) {
/*
        Function to read one word from given port address and return it.
        input size: 2bytes
        output size: 2byte
*/
        unsigned short int result;    	//Allocate 2 bytes for return value.
        __asm__("in %1, %0"             //GAS syntax. Use DX address as port address and put result in AX.
                : "=a" (result)         //Use AX value as output value and put it in result variable.
                : "d" (port)            //Use DX as input register. Set DX to port variable value.
                );                      //No clobbered registers, GCC knows them as we told it which we use.

        return result;
}

void portIO_word_write(unsigned short int port, unsigned short int byte) {
/*
        Function to write one word at given port address.
        input size: 2bytes, 2bytes
*/
        __asm__("out %%ax, %%dx"                //GAS syntax. Write AX to address in DX.
                :                               //No output
                : "a" (byte), "d" (port)        //byte -> AX, port -> DX
                );                              //No clobbered registers.
}
