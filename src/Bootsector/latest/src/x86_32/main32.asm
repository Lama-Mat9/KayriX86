;
;   Protected mode entry point.
;

; This is the main function to call when we have successfully left real mode
main32:

    ; ---- Print protected mode msg ----
    mov ebx, PM_STRING
    mov al, 0x0             ; Set the message's X position
    mov ah, 24              ; Set the message's Y position
    call prints32

    call KERNEL_ADDRESS     ; Jump to our kernel code

    jmp $           ; Jump forever. Program stops there

%include "src/x86_32/print32.asm"

; Global data defined here
PM_STRING: db "Protected mode: OK", 0

; Leave 0s until the end of the second sector
times 1024-($-$$) db 0