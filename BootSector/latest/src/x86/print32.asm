; Constant values
VIDEO_MEMORY equ 0x0b8000       ; The start of video memory
WHITE_ON_BLACK equ 0x07         ; Character attributes that we will use

; Print string (32 Bit addresses supported). Ends at 0x0 line ending
; Goes from low to high addresses
prints32:

; Note:

    ; This function is meant to be called in protected mode.
    ; This function assumes the GPU is set to 80 x 25 tty video mode.
    ; X = 0, Y = 0 is the first line on the top left of the screen.

; Parameters:

    ; EBX: Start of the string
    ; al: X Position to use
    ; ch: Y Position to use

; Usage example:

    ; mov ebx, PM_STRING
    ; mov al, 0x0
    ; mov ch, 0x0
    ; call prints32

; ---------------------------------------------------------------------------------------------------

    mov edx, VIDEO_MEMORY   ; Set EDX to the start of video memory

    ; We need to move forward in video memory to write to the X and Y positions that we have.
    ; We know that every two bytes is one character, and the screen is 80 x 25. 
    ; Therefore, an offset of 2 * Y * 80 + 2 * X should put us to the right position.

    ; ---- Adding 2 * X ----
    xor ah, ah      ; Make sure that AH is empty
    mov cl, 0x2     ; Need a general purpose register to hold 2 for MUL
    mul cl          ; Multiply AX (implicitly) with CL (value of 2)
    add edx, eax    ; Add the result to our pointer in video memory

    ; ---- Adding 2 * Y * 80 ----
    xor eax, eax      ; Make sure that AX is empty
    mov al, ch      ; Put Y position in AL to multiply AX
    mov cl, 160     ; 2 * 80 = 160. We hold it here for mul
    mul cl          ; AX * CL = 160 * Y
    add edx, eax

    mov ah, WHITE_ON_BLACK  ; Store the character attributes in AH
                            ; If we wrote text in it, it would be on the top left of the screen.

    .loop:
    ; We put the full current character in AX
    mov al, [ebx]           ; Move character at EBX to AL

    mov [edx], ax   ; Write full current character to video memory

    add ebx, 1  ; Move to next character in the string
    add edx, 2  ; Move to next character in video memory

    mov al, [ebx]   ; Put next char in al just for the comparaison
    cmp al, 0        ; If next character is not a string delimiter
    jne .loop  ; Print the next character


    ret     ; Return to caller