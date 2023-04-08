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

    jmp $           ; Jump forever. Program stops there

%include "src/x86/print32.asm"

; Global data defined here
PM_STRING: db "PM: OK", 0