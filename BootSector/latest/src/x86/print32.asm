; Constant values
VIDEO_MEMORY equ 0x000b8000    ; The start of video memory
WHITE_ON_BLACK equ 0x0007

; Print string (32 Bit addresses supported). Ends at 0x0 line ending
; Goes from low to high addresses
; Only prints to the top-left of the screen. Will only use it for debug anyways
prints32:

; Note:

    ; This function is meant to be called in protected mode.

; Parameters:

    ; EBX: Start of the string

; Usage example:

    ; mov ebx, STRING1
    ; call prints32

; ---------------------------------------------------------------------------------------------------

    mov edx, VIDEO_MEMORY   ; Set EDX to the start of video memory
    
    .prints32_loop:
    ; We put the full current character in AX
    mov al, [ebx]           ; Move character at EBX to AL
    mov ah, WHITE_ON_BLACK  ; Store the attributes in AH

    mov [edx], ax   ; Write full current character to video memory

    add ebx, 1  ; Move to next character in the string
    add edx, 2  ; Move to next character in video memory

    mov al, [ebx]   ; Put next char in al just for the comparaison
    cmp al, 0        ; If next character is not a string delimiter
    jne .prints32_loop  ; Print the next character


    ret     ; Return to caller