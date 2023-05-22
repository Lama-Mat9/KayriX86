#ifndef PORT_IO_H
#define PORT_IO_H

unsigned char portIO_byte_read(unsigned short int port);
void portIO_byte_write(unsigned short int port, unsigned char byte);
unsigned short int portIO_word_read(unsigned short int port);
void portIO_word_write(unsigned short int port, unsigned short int byte);
void portIO_wait();

#endif
