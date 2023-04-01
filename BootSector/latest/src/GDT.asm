;
;   This file contains the OS's GDT (Global Descriptor table)
;


; First 16 bits: The size of the GDT
; Next 32 bits: Pointer to the start of the GDT
gdt_descriptor: 
    ; Label referring to where we store the GDT descriptor structure
    ; Address to be loaded in the GDTR, never called

    ; Size of the GDT (In bytes) always substracted by 1
    ;                                   (that's the rule)
    dw gdt_end - gdt_start - 1  ; 2 x 8 bits = 16 bit size value

    ; Address of the start of our GDT
    dd gdt_start    ; Quadword: 2 * 2 bytes = 32 bits


; Start of where we put our GDT entries
gdt_start:
    gdt_null:   ; Mandatory empty 8 byte GDT entry
    dd 0x0          ; Define 4 empty bytes (Doubleword)
    dd 0x0          ; Define 4 empty bytes (Doubleword)
    
    gdt_code:   ; Code segment entry
    dw 0xffff       ; SEGMENT LIMIT START (bit 0 to 15)
    dw 0x0          ; BASE ADDRESS START (bit 0 to 15)

    db 0x0          ; BASE ADDRESS (bit 16 to 23)
    db 10011010b  ; ACCESS BYTE (P,DPL,S,E,DC,RW,A)
    ; Flags:
    ; P: Is a valid segment
    ; DPL: Has maximum privileges (Two bit long)
    ; S: Is a system segment
    ; E: Is a code segment
    ; DC: Need same or more privileges to execute contents of this segment
    ; RW: Read access is allowed
    ; A: Bit that has to be set to 0

    db 11001111b  ; FLAGS (G, DB, L, RESERVED), SEGMENT LIMIT (bit 16 to 19)
    ; Flags:
    ; G: Scale SEGMENT LIMIT by 4kb pages
    ; DB: Is a 32 bit protected mode segment
    ; L: We don't want long mode (64 bits)
    ; RESERVED: Reserved. Has to be 0

    db 0x0          ; BASE ADDRESS (24 to 31)

    gdt_data:   ; Data segment entry
    dw 0xffff       ; SEGMENT LIMIT START (bit 0 to 15)
    dw 0x0          ; BASE ADDRESS START (bit 0 to 15)

    db 0x0          ; BASE ADDRESS (bit 16 to 23)
    db 10010110b  ; ACCESS BYTE (P,DPL,S,E,DC,RW,A)
    ; Flags:
    ; P: Is a valid segment
    ; DPL: Has maximum privileges (Two bit long)
    ; S: Is a system segment
    ; E: Is a data segment
    ; DC: Segment grows down
    ; RW: Write access is allowed
    ; A: Bit that has to be set to 0

    db 11001111b  ; FLAGS (G, DB, L, RESERVED), SEGMENT LIMIT (bit 16 to 19)
    ; Flags:
    ; G: Scale SEGMENT LIMIT by 4kb pages
    ; DB: Is a 32 bit protected mode segment
    ; L: We don't want long mode (64 bits)
    ; RESERVED: Reserved. Has to be 0

    db 0x0          ; BASE ADDRESS (24 to 31)

gdt_end:
; No more GDT entries after this label

; Constants to use the GDT once loaded
CODE_SEG equ gdt_code - gdt_start   ; Offset (Index) of code entry
DATA_SEG equ gdt_data - gdt_start   ; Offset (Index) of data entry
; Once the GDT is loaded, segment registers must be set to the index
; of the GDT entry they correspond to. This index is the offset
; in bytes of the desired entry from the start of the GDT.
; For example, the null entry is at offset 0. The first entry is
; at 0x8 and the second at 0x10 (16) because each entry is 8 bytes.
; We are defining these constants so that we don't have to hardcode values everywhere.