#include <stdint.h>

#ifndef PIC_H
#define PIC_H

void PIC_init(uint8_t masterOffset, uint8_t slaveOffset);
void PIC_IRQ_enable(int irq);
void PIC_IRQ_disable(int irq);
void PIC_disableAll();
uint16_t PIC_ReadIRR();
uint16_t PIC_ReadISR();
uint16_t PIC_getBitmask();

#endif