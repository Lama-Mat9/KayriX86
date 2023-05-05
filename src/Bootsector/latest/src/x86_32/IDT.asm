;
;	Interrupt descriptor table
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