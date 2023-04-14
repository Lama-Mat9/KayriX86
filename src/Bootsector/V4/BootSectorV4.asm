;
;   Simple boot sector program (Version 4)
;   Implements CRLF strings and printing strings to screen in the bios's tty routine.
;   Note: It is not recommended to run this code on actual hardware, but rather emulate a machine using QEMU or something
;

mov ah, 0x0e ; int 10/ ah = 0 eh -> scrolling teletype BIOS routine

mov esi, string1 ; The reference to the string we wanna print
add esi, 0x7c00 ; Make the relative address to string absolute
call print

mov esi, string2
add esi, 0x7c00
call print


jmp $ ; Jump forever. Code stops there


print: ; ESI: Start of the string.
    mov al, [esi]
    inc esi
    int 0x10

    mov bx, 0x0A0D  ; The CRLF signal that ends our string
    mov dh, [esi]   ; Should be 0x0A at the end of string
    
    inc esi    
    mov dl, [esi]   ; Should be 0x0D at the end of string
    dec esi

    cmp dx, bx
    jne print

    ret ; Return to caller if we finished printing all characters


; Data is being defined here
; I know that we should not use db and dw for strings that are this large. I have no other way soo...
string1:    ; Our strings use CRLF line endings, which means that they signify that they have ended when they find CR (0x0A) and LF (0x0D) 
    db "Hello world!"
    dw 0x0D0A ; Note: This is 0x0A0D on disk, but x86 works in little endian [smaller values right, bigger left] which inverts code
string2:
    dw "This boot sector implements a way of printing strings in assembly, which allows me to print long sentences fairly easily."
    dw 0x0D0A ; Note: This is 0x0A0D on disk, but x86 works in little endian [smaller values right, bigger left] which inverts code


; Padding and magic BIOS number.
times 510-($-$$) db 0
dw 0xaa55