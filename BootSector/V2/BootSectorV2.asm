;
; A simple boot sector program that loops forever.
;

loop:                   ; Define a label , " loop " , that will allow
    jmp loop            ; us to jump back to it , forever.
                        ; Use a simple CPU instruction that jumps
                        ; to a new memory address to continue execution.
                        ; In our case , jump to the address of the current
                        ; instruction.

; db 0:         nasm COMMAND. "define byte 0". Actually writes the given byte directly to our target file. 
; $:            the address of the instruction that is executed currently
; $$:           the start address of the current section. .code ?
; $-$$:         an offset. From where our program starts to the line that we are reading currently (How many lines we've read)
; 510-($-$$):   offset. From the last line we've read to line 510. Gives us how many lines left until line 510
; times:        nasm specific COMMAND. Tells nasm to assemble given NASM command multiple times
;                   This means that db 0 is executed multiple times by THE COMPILER, NASM. NO "TIMES" IN OUTPUT FILE, NOR IN ASSEMBLY LANGUAGE.
;                   

times 510-($-$$) db 0   ; When compiled , our program must fit into 512 bytes ,
                        ; with the last two bytes being the magic number ,
                        ; so here , tell our assembly compiler to pad out our
                        ; program with enough zero bytes ( db 0) to bring us to the
                        ; 510 th byte.

; dw:           nasm COMMAND. "define word". Actually writes the given word directly to our target file

dw 0xaa55               ; Last two bytes ( one word ) form the magic number ,
                        ; so BIOS knows we are a boot sector.