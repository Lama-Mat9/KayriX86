/*
	Provides an interface to use the PIC's features easily.
*/

#include <stdint.h>
#include "pic.h"
#include "microclib/port_io.h"

//8259 PIC standard port addresses
#define PIC1_COMMAND_PORT 0x20	//-> A0 = 0. Master PIC.
#define PIC1_DATA_PORT 0x21		//-> A0 = 1. Master PIC.
#define PIC2_COMMAND_PORT 0xA0	//-> A0 = 0. Slave PIC.
#define PIC2_DATA_PORT 0xA1		//-> A0 = 1. Slave PIC.

/*
	Recommanding intel's "8259A PROGRAMMABLE INTERRUPT CONTROLLER (8259A/8259A-2)" document for details.
*/

void PIC_init(uint8_t masterOffset, uint8_t slaveOffset) {
/*
	Configures the PIC to use the given offsets for the interrupts that it will generate.
	Note: All IRQs remain disabled by default.
*/

	//-----------------------ICW1---------------------------------------------------------

	//Send initialisation command word 1 (ICW1) to master PIC
	portIO_byte_write(PIC1_COMMAND_PORT, 0x11);
	portIO_wait();	//Old PICs could need some time before being reachable again.

	//Send initialisation command word 1 (ICW1) to slave PIC
	portIO_byte_write(PIC2_COMMAND_PORT, 0x11);
	portIO_wait();

		/*
			0x11 -> 00010001
			Settings:
				D7	0: Not for 8086 mode
				D6	0: Not for 8086 mode
				D5	0: Not for 8086 mode
				D4	1: When A0 = 0 and D4 = 1 it signals PIC that this is ICW1
				D3	0: Edge triggered mode
				D2	0: Call address interval of 8
				D1	0: Cascade mode (We are using two PICs, master and slave)
				D0	1: We require and will send an ICW4.
		*/

	//-----------------------ICW2---------------------------------------------------------

	//Send ICW2 to master PIC
	portIO_byte_write(PIC1_DATA_PORT, masterOffset & 0xF8);
	portIO_wait();

	//Send ICW2 to slave PIC
	portIO_byte_write(PIC2_DATA_PORT, slaveOffset & 0xF8);
	portIO_wait();

	/*
		ICW2 has it's three least significant bits unusable in 8086 mode.
		This is why we AND the input with 0xF8 -> 11111000 to make sure the last three are clear.
		The five most significant bits are the minimal interrupt number that the PIC will use.
		As each PIC has eight IRQs to service, it will use this provided number for it's lowest IRQ
												to this number +8 for it's highest IRQ.
	*/

	//-----------------------ICW3---------------------------------------------------------

	//Send ICW3 to master PIC
	portIO_byte_write(PIC1_DATA_PORT, 0x04);
	portIO_wait();

	/*
		0x04 -> 00000100
		Settings:
			D7	0: No slaves connected on pin 7
			D6	0: No slaves connected on pin 6
			D5	0: No slaves connected on pin 5
			D4	0: No slaves connected on pin 4
			D3	0: No slaves connected on pin 3
			D2	1: Slave connected on pin 2
			D1	0: No slaves connected on pin 1
			D0	0: No slaves connected on pin 0
			
	*/

	//Send ICW3 to slave PIC
	portIO_byte_write(PIC2_DATA_PORT, 0x02);
	portIO_wait();

	/*
		0x02 -> 2
		We send 2 so that the slave knows that it is connected to pin 2 of master PIC.
	*/

	//-----------------------ICW4---------------------------------------------------------

	//Send ICW4 to master PIC
	portIO_byte_write(PIC1_DATA_PORT, 0x03);
	portIO_wait();

	//Send ICW4 to slave PIC
	portIO_byte_write(PIC2_DATA_PORT, 0x03);
	portIO_wait();


	/*
		0x03 -> 00000011
		Settings:
			D7	0: Required to be 0
			D6	0: Required to be 0
			D5	0: Required to be 0
			D4	0: Not special fully nested mode
			D3	0: Non buffered mode
			D2	0: Non buffered mode
			D1	1: Automatic end of interrupts (EOI) ON
			D0	1: 8086 mode (because x86 arch)	
	*/

	//-----------------------CLEANUP---------------------------------------------------------

	//Disable all IRQs so that they can be manually enabled when needed.
	PIC_disableAll();

}

uint16_t PIC_getBitmask() {
/*
	Returns both of the PICs bitmask combined.
*/

	return (uint16_t) (portIO_byte_read(PIC2_DATA_PORT) << 8) | (portIO_byte_read(PIC1_DATA_PORT));
}

void PIC_IRQ_enable(int irq) {
/*
	Unmasks the provided IRQ.
	This means that when the PIC needs to raise a CPU interrupt about this IRQ, it will do it. 
*/

	//If the IRQ belongs to master PIC
	if (irq < 8) {

		//We read the current bitmask to not overwrite what was there
		uint8_t current_bitmask = portIO_byte_read(PIC1_DATA_PORT);

		//Each bit being it's own IRQ, we have to set bit at the "irq" position to 0.
		current_bitmask &= ~(1UL << irq);

		//Switch off the bit corresponding to the IRQ pin to enable on the current mask.
		portIO_byte_write(PIC1_DATA_PORT, current_bitmask);	
	}
	else {	//If the IRQ belongs to slave PIC


		//Map irq 8 -> 15 to 0 -> 8 so that we can make it an 8 bit bitmask for the slave PIC
		irq -= 8;

		//We read the current bitmask to not overwrite what was there
		uint8_t current_bitmask = portIO_byte_read(PIC2_DATA_PORT);	

		//Each bit being it's own IRQ, we have to set bit at the "irq" position to 0.
		current_bitmask &= ~(1UL << irq);

		//Switch off the bit corresponding to the IRQ pin to enable on the current mask.
		portIO_byte_write(PIC2_DATA_PORT, current_bitmask);	
	}

}

void PIC_IRQ_disable(int irq) {
/*
	Masks the provided IRQ.
	This means that when the PIC needs to raise a CPU interrupt about this IRQ, it won't do it.
*/

	//If the IRQ belongs to master PIC
	if (irq < 8) {

		//Each bit being it's own IRQ, we have to set bit at the "irq" position to 1.
		uint8_t irqPinBit = 1 << irq;

		//We read the current bitmask to not overwrite what was there
		uint8_t current_bitmask = portIO_byte_read(PIC1_DATA_PORT);

		//Switch on the bit corresponding to the IRQ pin to disable on the current mask.
		portIO_byte_write(PIC1_DATA_PORT, current_bitmask | irqPinBit);	
	}
	else {	//If the IRQ belongs to slave PIC


		//Map irq 8 -> 15 to 0 -> 8 so that we can make it an 8 bit bitmask for the slave PIC
		irq -= 8;

		//Each bit being it's own IRQ, we have to set bit at the "irq" position to 1.
		uint8_t irqPinBit = 1 << irq;

		//We read the current bitmask to not overwrite what was there
		uint8_t current_bitmask = portIO_byte_read(PIC2_DATA_PORT);	

		//Switch on the bit corresponding to the IRQ pin to disable on the current mask.
		portIO_byte_write(PIC2_DATA_PORT, current_bitmask | irqPinBit);	
	}
	
}

void PIC_disableAll() {
/*
	Masks all IRQs.
	The PICs will stop interrupting the CPU for all IRQs.
*/

	portIO_byte_write(PIC1_DATA_PORT, 0xFF);
	portIO_wait();

	portIO_byte_write(PIC2_DATA_PORT, 0xFF);
	portIO_wait();

	/*
		Both PICs hold a bitmask which indicate which IRQs should not be sent to the CPU and which should.
		By setting their mask to 0xFF -> 11111111 we are disabling all IRQs.
	*/
}

uint16_t PIC_ReadIRR() {
/*
	Reads the PIC's Interrupt Request Register (IRR).
	It is a bitmask of all the pending interrupts.
	If bit at position X is set to 1 it means that IRQX is waiting to be serviced.

	Returns the content of the master PIC's IRR as most significant byte
		followed by the content of the slave PIC's IRR as least significant byte.
*/

	//A0 = 0 with D4 = 0 and D3 = 1 signals OCW1. OCW1 allows us to request the IRR by providing D1 = 1, D0 = 0.
	//Therefore requiring us to send 00001010 -> 0x0A as we don't care about the other bits.

	//We send the IRR request to both master and slave PIC.
	portIO_byte_write(PIC1_COMMAND_PORT, 0x0A);
	portIO_byte_write(PIC2_COMMAND_PORT, 0x0A);

	//This write makes the IRR available "on next read pulse".
	//This means that we can now read it.

	//Combine both reads into a 16 bit bitmask, having the master PIC's IRR shifted 8 bits to the left.
	uint16_t irr = (portIO_byte_read(PIC2_COMMAND_PORT) << 8) | portIO_byte_read(PIC1_COMMAND_PORT);

	return irr;
}

uint16_t PIC_ReadISR() {
/*
	Reads the PIC's In-Service Register (ISR).
	It is a bitmask of all interrupts that are currently being serviced by the CPU.
	If bit at position X is set to 1 it means that IRQX is waiting for it's EOI (End Of Interrupt).

	Returns the content of the master PIC's ISR as most significant byte
		followed by the content of the slave PIC's ISR as least significant byte.
*/

	//A0 = 0 with D4 = 0 and D3 = 1 signals OCW1. OCW1 allows us to request the ISR by providing D1 = 1, D0 = 1.
	//Therefore requiring us to send 00001011 -> 0x0B as we don't care about the other bits.

	//We send the ISR request to both master and slave PIC.
	portIO_byte_write(PIC1_COMMAND_PORT, 0x0B);
	portIO_byte_write(PIC2_COMMAND_PORT, 0x0B);

	//This write makes the ISR available "on next read pulse".
	//This means that we can now read it.

	//Combine both reads into a 16 bit bitmask, having the master PIC's IRR shifted 8 bits to the left.
	uint16_t isr = (portIO_byte_read(PIC2_COMMAND_PORT) << 8) | portIO_byte_read(PIC1_COMMAND_PORT);

	return isr;
}