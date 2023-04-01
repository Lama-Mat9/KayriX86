;
;   Protected mode entry point.
;

; This is the main function to call when we have successfully left real mode
main32:

    ; Protected mode enabled message
    mov ebx, PM_STRING
    call prints32

    jmp $           ; Jump forever. Program stops there

%include "src/x86/print32.asm"

; Global data defined here
PM_STRING: db "Protected mode is now ON", 0