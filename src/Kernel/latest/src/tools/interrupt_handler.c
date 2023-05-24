#include <stdint.h>
#include "microclib/stdio/printf.h"

//Local struct that is used to identify registers passed in order from asm generic handler to here.
typedef struct cpu_registers {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    //Compiler should not insert padding to args.
} __attribute__((packed)) cpu_registers;

//Local struct containing information about the interrupt
typedef struct interrupt_info {
    uint32_t interrupt_number;
    uint32_t error_code;
    uint32_t eip;           //A copy of EIP made at interrupt time, not the current one.
    uint32_t cs;            //A copy of CS made at interrupt time, not the current one.
    uint32_t eflags;

    //Compiler should not insert padding to args.
} __attribute__((packed)) interrupt_info;

//Generic asm interrupt handler passes control to this function, along with some parameters.
void kernel_interrupt_handler(cpu_registers cpu, interrupt_info interrupt_information) {

    printf("Kernel received interrupt {d}\n", interrupt_information.interrupt_number);

}
