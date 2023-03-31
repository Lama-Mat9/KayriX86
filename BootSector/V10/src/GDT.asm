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
    ; Mandatory empty 8 byte GDT entry
    dd 0x0      ; Define 4 empty bytes (Doubleword)
    dd 0x0      ; Define 4 empty bytes (Doubleword
    
    gdt_code:   ; Code segment entry


    gdt_data:   ; Data segment entry

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