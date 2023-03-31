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

gdt_start:

gdt_end:
