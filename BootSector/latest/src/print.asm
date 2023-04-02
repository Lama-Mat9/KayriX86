; Print string (16 Bit addresses supported). Ends at 0x0 line ending
; Goes from low to high addresses
prints: ;

; Note:

    ; This function is meant to be called in real mode.

; Parameters:

    ; BX: Start of the string.

; Usage example:

    ; mov bx, STRING1
    ; call prints

; ---------------------------------------------------------------------------------------------------

    mov ah, 0x0e ; int 10/ ah = 0 eh -> scrolling teletype BIOS routine

    mov al, [bx]    ; Stores current char for sys interrupt         
    int 0x10        ; Sys interrupt that prints char in AL

    inc bx  ; Move forward 1 character for the next prints

    mov cl, 0x00    ; Used to compare string char to 0 to see it it's the end
    cmp [bx], cl      ; Compare next char with usual line ending
    jne prints      ; Jump if next char is not the end of the string

    ret ; Return to caller if we finished printing all characters

; Print a 2 byte long hex value as ASCII
printh: 

; Reminder:

    ; The stack goes from high to low addresses. While prints goes from low to high addresses
    ; This means that this function will reverse bytes so that they are printed correctly

; Parameters:

    ; DX: The hexadecimal value to print

; Usage example:
    
    ;mov dx, 0x0000
    ;call printh

; ------------------------------------------------------------------------------------------------------
    
    ; We do all of our pushes there so that we can use BP/SP ourselves afterwards
    push bx
    push bp

    ; Keep SP at BP so that we can free non push and popped values on the stack later
    mov bp, sp 


    ; Separate each 4 bit hex component into 8 bit registers
    ; This way we can convert them later by adding stuff 
    mov al, dl      ; Store DL in AL so that we can use the 4 lsb bits separately
    mov ah, dh      ; Same operation with DH and AH. (DH -> AH, DL -> AL)
    
    mov cl, 0x0F     ; Base 2 '00001111'. Register used for the  AND operation
    and al, cl      ; Keep only the 4 least signifcant bits of DL in AL
    and ah, cl      ; Keep only the 4 least signifcant bits of DH in AH

    shr dl, 4   ; Keep only the 4 most significant bits of DL
    shr dh, 4   ; Keep only the 4 most significant bits of DH


    ; Convert all separate letters to ASCII by adding:
    ; 0x30 if the component is between [0x0 to 0x9]
    ; 0x7 + 0x30 (=0x41) if the component is between [0xA to 0xF]
    mov cl, 0xA         ; We use CL as comparaison register to check if below 0xA

    ; We repeat the same comparaison process 4 times, once per character
    cmp dl, cl          ; Compare DL and 0xA
    jb .l1              ; If DL below 0xA (Unsigned comparaison), skip the next line
    add dl, 0x7         ; DL was over 0xA, so we add 0x7 
    .l1: add dl, 0x30   ; Add 0x30 no matter the value in DL

    cmp dh, cl
    jb .l2
    add dh, 0x7
    .l2: add dh, 0x30

    cmp al, cl
    jb .l3
    add al, 0x7
    .l3: add al, 0x30

    cmp ah, cl
    jb .l4
    add ah, 0x7
    .l4: add ah, 0x30


    ; Now we have to put the characters back in order in DX and AX (for little endian)
    ; We will fully overwrite CX to use it as temporary means of storage
    mov cl, dl  ; Put these two bytes in CX so that DL and AH are free to fill
    mov ch, ah  ; them with DH and AL respectively

    shr dx, 8   ; Slide DH to DL
    shl ax, 8   ; Slide AL to AH

    mov dh, ch  ; Put back the temporary CX bytes 
    mov al, cl  ; where they should normally be 


    ; Finally put the values in order on the stack
    ; so that SP points to the start of the string
    ; (Value pushed last will be printed first because stack)
    push 0x0   ; Line ending
    
    push ax     ; Last two ASCII bytes
    push dx     ; First two ASCII bytes

    push "0x"   ; Start of the string


    ; Now we can call prints to print ASCII
    mov bx, sp   ; We pass the address of the top of the stack 
    call prints  ; as start of our string


    ; Free non push / popped values on the stack by restoring SP
    mov sp, bp

    ; We pop the values that we have used push on earlier
    pop bp
    pop bx

    ret

; Sets cursor to the start of next line
newline:
    push 0x0        ; End of our characters to print (Two bytes)
    push 0x0A0D     ; Newline and carriage return code pushed to the stack
    mov bx, sp      ; Give the stack as parameter to prints
    call prints
    add sp, 4       ; Free the four bytes that we've used

    ret