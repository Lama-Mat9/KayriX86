print: ; ECX: Start of the string.  Uses FASTCALL convention.
    mov ah, 0x0e ; int 10/ ah = 0 eh -> scrolling teletype BIOS routine

    mov al, [ecx]
    inc ecx
    int 0x10

    push ebx ; Preserve EBX by putting it on top of stack
    mov bx, 0x0A0D  ; The CRLF signal that ends our string
    mov dh, [ecx]   ; Should be 0x0A at the end of string
    
    inc ecx    
    mov dl, [ecx]   ; Should be 0x0D at the end of string
    dec ecx

    cmp dx, bx
    pop ebx ; Restore EBX in any case
    jne print

    ret ; Return to caller if we finished printing all characters
