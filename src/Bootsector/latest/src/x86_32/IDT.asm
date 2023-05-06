;
;	Interrupt descriptor table (with their handlers)
;

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

    ; Interrupt 0: Division by 0
    dw 0x0                          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR

    ; ---- This part is only copies of int 0. ----
    ; They are only there temporarly so that the CPU doesn't fault when seeing that we load an IDT of 1 entry
    ; I will replace them with valid ones later

        ; Interrupt 0: Division by 0
    dw 0x0                          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR
        ; Interrupt 0: Division by 0
    dw 0x0                          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR
        ; Interrupt 0: Division by 0
    dw 0x0                          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR
        ; Interrupt 0: Division by 0
    dw 0x0                          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR
        ; Interrupt 0: Division by 0
    dw 0x0                          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR
        ; Interrupt 0: Division by 0
    dw 0x0                          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR
        ; Interrupt 0: Division by 0
    dw 0x0                          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR
        ; Interrupt 0: Division by 0
    dw 0x0                          ; Least significant double word for the address of the ISR
    dw 0000000000001000b            ; Segment selector. Uses first GDT entry and highest privileges.
    db 0x0                          ; Reserved. Set to 0 then.
    db 10001110b                    ; (1, DPL, 0, Gate type)
    dw 0x0                          ; Most significant double word for the address of the ISR
    ; Interrupt 0: Division by 0
    dw 0x0                          ; Least significant double word for the address of the ISR
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

; Generic interrupt handler. 
; Passes control and parameters to the kernel.
interrupt_handler:

; Note:

    ;   Uses iret instruction directly, so macros don't have to.
    ;   Do not call the handler, it is meant to be jumped to.

; Parameters:

    ;   [ESP]: Interrupt number
    ;   [ESP+4]: Error code

; ---------------------------------------------------------------------------------------------------
    ; Save all registers for security because we cannot know
    ; what the C function will do to them.
    pusha

    ; Call the generic C interrupt handler function
    call interrupt_handler

    ; Restore all registers
    popa

    ; Free stack parameters (two doublewords)
    add esp, 8

    iret    ; Returns using iret, so that the macros don't have to handle a return
            ; because it will directly jump back to the code that generated the interrupt.