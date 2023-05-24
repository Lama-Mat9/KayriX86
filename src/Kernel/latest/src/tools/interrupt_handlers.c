#include <stdint.h>
#include "microclib/stdio/printf.h"
#include "drivers/PIC/pic.h"


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

void kernel_panic(cpu_registers cpu, interrupt_info interrupt_information);
void print_info(cpu_registers cpu, interrupt_info interrupt_information);

//Generic asm interrupt handler passes control to this function, along with some parameters.
void kernel_interrupt_handler(cpu_registers cpu, interrupt_info interrupt_information) {

    //If any exeption is raised. Will need to handle some exeptions later
    if (interrupt_information.interrupt_number < 32) kernel_panic(cpu, interrupt_information);
    else {
        printf("Kernel received interrupt {d}\n", interrupt_information.interrupt_number);
    }
    
}

//  ---- REAL INTERRUPT HANDLERS ----

void kernel_panic(cpu_registers cpu, interrupt_info interrupt_information) {
/*
    Generates a simple kernel panic.
    Will dump all information and stop execution.

    Should normally not be depending on any drivers. Will look at that later. 
*/

    printf("KERNEL PANIC !\n", interrupt_information.interrupt_number);
    print_info(cpu, interrupt_information);
    printf("Execution will not continue. You can shut down the system.");

    //Remove the ability for an interrupt to remove us from the loop
    PIC_disableAll();

    //Kill the thread forever
    for (;;);
    
}

void print_info(cpu_registers cpu, interrupt_info interrupt_information) {
/*
    Prints all of the structs information.
*/
    printf("EAX: {b}\nECX: {b}\nEDX: {b}\nEBX: {b}\nESP: {b}\nEBP: {b}\nESI: {b}\nEDI: {b}\n", cpu.eax, cpu.ecx, cpu.edx, cpu.ebx, cpu.esp, cpu.ebp, cpu.esi, cpu.edi);

    printf("Interrupt number: {d}\n", interrupt_information.interrupt_number);
    printf("Error code: {d}\n", interrupt_information.error_code);
    printf("EIP: {x}\n", interrupt_information.eip);
    printf("CS: {x}\n", interrupt_information.cs);
    printf("EFLAGS: {b}\n", interrupt_information.eflags);
}