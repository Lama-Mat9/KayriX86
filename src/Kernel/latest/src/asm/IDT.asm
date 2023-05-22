;
;	Interrupt descriptor table (with its handler and loader)
;

global idt_load     ; Must be accessible to the kernel
extern kernel_interrupt_handler    ; Kernel function to be accessed from here

; Function that loads the IDT and re-enables interrupts.
; This was meant to be called from the kernel.
idt_load:

    lidt [idt_descriptor]   ; Load IDT descriptor into IDTR
    sti                     ; Set interrupt flag: Re-enable interrupts so that
                            ; our kernel can use our new 32 bit interrupts listed in the IDT.

    ret

; First 16 bits: The size of our IDT
; Next 32 bits: The linear address of our IDT
idt_descriptor:     ; This is what we will load in the IDTR, never call.

    ; Size of the IDT (In bytes) has to be always substracted by 1
    ; because a size of 0 results in one valid byte (page 203 of
    ; "Intel® 64 and IA-32 Architectures Software Developer’s Manual Vol 3A")
    dw idt_end - idt_start - 1

    ; Address of the start of our IDT
    dd idt_start



; Start of where we put the IDT entries
idt_start:

    ; For information about the first (reserved) interrupts
    ; https://en.wikipedia.org/wiki/Interrupt_descriptor_table

    ;                   ==== Interrupt 0: Division by 0 ====
    dw interrupt_handler_0          ; Least significant double word for the address of the ISR
                                        ; This technique will only let us index interrupt handlers placed 
                                        ; in the first 64K of memory.

    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 1: Single-step interrupt ====
    dw interrupt_handler_1          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 2: NMI ====
    dw interrupt_handler_2          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 3: Breakpoint ====
    dw interrupt_handler_3          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 4: Overflow ====
    dw interrupt_handler_4          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 5: Bound Range Exceeded ====
    dw interrupt_handler_5          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 6: Invalid Opcode ====
    dw interrupt_handler_6          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 7: Coprocessor not available ====
    dw interrupt_handler_7          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 8: Double Fault ====
    dw interrupt_handler_8          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 9: Coprocessor Segment Overrun ====
    dw interrupt_handler_9          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 10: Invalid Task State Segment ====
    dw interrupt_handler_10          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 11: Segment not present ====
    dw interrupt_handler_11          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 12: Stack Segment Fault ====
    dw interrupt_handler_12          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 13: General Protection Fault ====
    dw interrupt_handler_13          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 14: Page Fault ====
    dw interrupt_handler_14          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 15: Reserved ====
    dw interrupt_handler_15          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 16: x87 Floating Point Exception ====
    dw interrupt_handler_16          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 17: Alignment Check ====
    dw interrupt_handler_17          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 18: Machine Check ====
    dw interrupt_handler_18          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 19: SIMD Floating Point Exception ====
    dw interrupt_handler_19          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 20: Virtualization Exception ====
    dw interrupt_handler_20          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 21: Control Protection Exception ====
    dw interrupt_handler_21          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 22: Reserved ====
    dw interrupt_handler_22          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 23: Reserved ====
    dw interrupt_handler_23          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 24: Reserved ====
    dw interrupt_handler_24          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 25: Reserved ====
    dw interrupt_handler_25          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 26: Reserved ====
    dw interrupt_handler_26          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 27: Reserved ====
    dw interrupt_handler_27          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 28: Hypervisor Injection Exception ====
    dw interrupt_handler_28          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 29: VMM Communication Exception ====
    dw interrupt_handler_29          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 30: Security Exception ====
    dw interrupt_handler_30          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== Interrupt 31: Reserved ====
    dw interrupt_handler_31          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR


    ; ======================================= Master PIC IRQs =======================================

    ;                   ==== IRQ 0: Programmable Interrupt Timer ====
    dw interrupt_handler_32         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 1: Keyboard Interrupt ====
    dw interrupt_handler_33         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 2: Cascade (Shouldn't ever be triggered) ====
    dw interrupt_handler_34         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 3: COM2 ====
    dw interrupt_handler_35         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 4: COM1 ====
    dw interrupt_handler_36         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 5: LPT2 ====
    dw interrupt_handler_37         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 6: Floppy Disk ====
    dw interrupt_handler_38         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 7: LPT1 / Spurious Interrupt ====
    dw interrupt_handler_39         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ; ======================================= Slave PIC IRQs =======================================

    ;                   ==== IRQ 8: CMOS Real Time Clock ====
    dw interrupt_handler_40         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 9: Peripherals / SCSI / NIC ====
    dw interrupt_handler_41         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 10: Peripherals / SCSI / NIC ====
    dw interrupt_handler_42         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 11: Peripherals / SCSI / NIC ====
    dw interrupt_handler_43         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 12: PS2 Mouse ====
    dw interrupt_handler_44         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 13: FPU / Coprocessor / Inter-processor ====
    dw interrupt_handler_45         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 14: Primary ATA Hard Disk ====
    dw interrupt_handler_46         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ;                   ==== IRQ 15: Secondary ATA Hard Disk ====
    dw interrupt_handler_47         ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

; No more IDT entries after this label
idt_end:


;       ---- Interrupt handlers definition ----

;
;   We chose that all interrupts would jump to almost the same handlers.
;   
;   There are two kinds of interrupts in this case, the ones which provide 
;   an error code on the stack (Type 1) and those who don't (Type 0).
;   
;   We handle these in a way that there is only one interrupt handler
;   defined in assembly, which will receive as parameter the interrupt vector
;   and the error code (error code 0 needs to be passed for no errors).
;   
;   This handler will jump to a generic interrupt handler that belongs to our C kernel,
;   and provides it details about the interrupt it needs to handle.

; Type 0 interrupt handler macro definition
%macro interrupt_handler_T0 1
interrupt_handler_%1:
    push    dword 0                     ; Push 0 as error code
    push    dword %1                    ; Push the interrupt number
    jmp     interrupt_handler           ; Jump to the real generic handler
%endmacro

; Type 1 interrupt handler macro definition
%macro interrupt_handler_T1 1
interrupt_handler_%1:                   ; Error code was already pushed by the CPU
    push    dword %1                    ; Push the interrupt number
    jmp     interrupt_handler           ; Jump to the real generic handler
%endmacro


; First 32 interrupts are CPU exceptions. We cannot remap them. 
; It is defined that the interrupts: 8, 10, 11, 12, 13, 14, 17      have error codes (Type 1)
; All the other interrupts                                          have no error codes (Type 0)
interrupt_handler_T0 0
interrupt_handler_T0 1
interrupt_handler_T0 2
interrupt_handler_T0 3
interrupt_handler_T0 4
interrupt_handler_T0 5
interrupt_handler_T0 6
interrupt_handler_T0 7
interrupt_handler_T1 8
interrupt_handler_T0 9
interrupt_handler_T1 10
interrupt_handler_T1 11
interrupt_handler_T1 12
interrupt_handler_T1 13
interrupt_handler_T1 14
interrupt_handler_T0 15
interrupt_handler_T0 16
interrupt_handler_T1 17
interrupt_handler_T0 18
interrupt_handler_T0 19
interrupt_handler_T0 20
interrupt_handler_T0 21
interrupt_handler_T0 22
interrupt_handler_T0 23
interrupt_handler_T0 24
interrupt_handler_T0 25
interrupt_handler_T0 26
interrupt_handler_T0 27
interrupt_handler_T0 28
interrupt_handler_T0 29
interrupt_handler_T1 30
interrupt_handler_T0 31

; Master PIC IRQ interrupt handlers
interrupt_handler_T0 32
interrupt_handler_T0 33
interrupt_handler_T0 34
interrupt_handler_T0 35
interrupt_handler_T0 36
interrupt_handler_T0 37
interrupt_handler_T0 38
interrupt_handler_T0 39

; Slave PIC IRQ interrupt handlers
interrupt_handler_T0 40
interrupt_handler_T0 41
interrupt_handler_T0 42
interrupt_handler_T0 43
interrupt_handler_T0 44
interrupt_handler_T0 45
interrupt_handler_T0 46
interrupt_handler_T0 47

; Generic interrupt handler. 
; Passes control and parameters to the kernel.
interrupt_handler:

; Note:

    ;   Uses iret instruction directly, so macros don't have to.
    ;   Do not call the handler, it is meant to be jumped to.

; Parameters:

    ;   [ESP]:      Interrupt number
    ;   [ESP+4]:    Error code

; CPU provided parameters (auto):

    ;   [ESP+8]:    EIP at interrupt time
    ;   [ESP+12]:   CS at interrupt time
    ;   [ESP+16]:   eflags

; ---------------------------------------------------------------------------------------------------

    ; Pass all registers to the C function in case that it needs to modify them.
    push eax
    push ecx
    push edx
    push ebx
    push esp
    push ebp
    push esi
    push edi

    ; Call the generic C interrupt handler function
    call kernel_interrupt_handler

    ; Save the C function's changes to the registers
    pop edi
    pop esi
    pop ebp
    pop esp
    pop ebx
    pop edx
    pop ecx
    pop eax


    ; Free stack parameters (two doublewords)
    add esp, 8

    jmp $   ; If we don't stop here, CPU will jump back to whatever has caused the interrupt,
                    ; therefore triggering the interrupt again.

    iret    ; Returns using iret, so that the macros don't have to handle a return
            ; because it will directly jump back to the code that generated the interrupt.