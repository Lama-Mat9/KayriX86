;
;   Protected mode entry point
;

; We are now in 32 bit protected mode
bits 32     

; This is the main function to call when we have successfully left real mode
main32:

    mov ebx, PM_STRING
    call prints32

    ;mov ebx, 0x000b8000
    ;mov eax, 0x07690748
    ;mov [ebx], eax


    jmp $           ; Jump forever. Program stops there

%include "src/x86/print32.asm"

; Global data defined here
PM_STRING: db "Enabled protected mode successfully", 0