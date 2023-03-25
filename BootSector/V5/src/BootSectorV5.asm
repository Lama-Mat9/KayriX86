;
;   Simple boot sector program (Version 5)
;   Improves the previous BootSector by making print.asm usable globally, and respecting FASTCALL while simplifying itself.
;   Note: It is not recommended to run this code on actual hardware, but rather emulate a machine using QEMU or something
;

[org 0x7c00]    ; Tell the assembler to translate relative addresses to absolute ones by adding the location where our program is expected to be loaded

mov ecx, string1 ; The reference to the string we wanna print
call print

mov ecx, string2
call print

jmp $ ; Jump forever. Code stops there

; Add print.asm code here
%include "src/print.asm"

; Data is being defined here
string1:    ; Our strings use CRLF line endings, which means that they signify that they have ended when they find CR (0x0A) and LF (0x0D) 
    db "Hello world!"
    dw 0x0D0A ; Note: This is 0x0A0D on disk, but x86 works in little endian [smaller values right, bigger left] which inverts code
string2:
    dw "This boot sector implements a way of printing strings in assembly, which allows me to print long sentences fairly easily."
    dw 0x0D0A ; Note: This is 0x0A0D on disk, but x86 works in little endian [smaller values right, bigger left] which inverts code


; Padding and magic BIOS number.
times 510-($-$$) db 0
dw 0xaa55