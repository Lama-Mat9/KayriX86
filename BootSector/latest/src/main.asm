;
;   KayriX86 boot sector program (latest)
;

org 0x7c00 ; Translate relative addresses to absolute ones
bits 16     ; We're using real mode in the boot sector

boot:   ; Program is read from top to bottom, so this will be the "entry point".
    jmp main    ; We directly jump to main as the rest are just commands for nasm to make a Bios Parameter Block (BPB)
    
    times 0x3E-($-$$) db 0  ; Empty BPB (DOS 4.0 EBPB). Space seems to be allocated until 0x3E. We keep it fully empty.

main:
    ; ---- Setup segmentation registers ----
    ; Segmentation registers are often used implicitly in instructions, even if we don't see them.
    ; For example, the MOV instruction will force us to use an offset from DS, and CMPSB forces DS and ES.
    ; The BIOS (on real hardware) doesn't seem to cleanup segment registers before it passes control to our OS.
    ; So we need to set some segmentation registers to 0 so that we can use linear addresses correctly. (Just in case). 
    xor ax, ax      ; Set AX to 0 so that we can set other registers to 0
    mov ds, ax      ; Set DS to 0
    mov es, ax      ; Set ES to 0
    mov fs, ax      ; Set FS to 0
    mov gs, ax      ; Set GS to 0
    mov ss, ax      ; Stack is relocated from 0x0

    mov bp, 0x8000      ; Put our stack frame far away so that we don't touch it accidentally
    mov sp, bp          ; Update SP too because we moved the stack frame

    ; ---- Collecting info ----
    mov [BOOT_DRIVE], dl    ; The BIOS should put our boot drive index in DL
                            ; We store now for later before someone erases it

    ; ---- Setup the display mode ----
    mov al, 0x03        ; 80 x 25 text tty. 16 Colors supported. SHOULD be 720x400 resolution but i'm not sure.
    mov ah, 0x0         ; "Set video mode" BIOS routine parameter
    int 0x10            ; "Video services" BIOS routine interrupt call "code"
    ; Note that it will also clear the screen for us. 
    ; For information about this interrupt, visit: https://en.wikipedia.org/wiki/INT_10H

    ; ---- Welcome message test ----
    mov bx, STRING1 ; Put the address of our string inside BX
    call prints     ; Print the string
    call newline


    ; ---- Print data from disk test ----
    mov bx, DISK_ID_MSG     ; Message that comes with drive ID print
    call prints

    mov dx, [BOOT_DRIVE]    ; Print the ID that the BIOS gave to our boot drive.
    call printh             ; Typically 0x80 for "first hard disk".
    call newline

    mov bx, DISK_TEST_MSG   ; Message that comes with drive test
    call prints

    mov al, 0x1            ; Load one sector
    mov dl, [BOOT_DRIVE]    ; From the drive we just booted from
    mov dh, 0               ; Using the disk's first head
    mov cl, 0x02            ; Starting from the first sector that isnt the boot sector (BootSector is at 0x01)
    mov ch, 0x00            ; With head on track 0
    mov bx, 0x9000          ; And copy result to ram at this address
    call read_dsk

    mov dx, [0x9000]        ; Retrieve the two first bytes at the first sector where we just wrote data
    call printh             ; And print these bytes

    ; Quit real mode to use 32 bit protected mode.
    ; Execution will never come back from this jump.
    jmp switch_pm
; -----------------------------------------------------------------------------------------------------

; Includes code here
%include "src/print.asm"
%include "src/read_dsk.asm"
%include "src/switch_pm.asm"

; Global data defined here

; Strings that we could print, 0 string end delimiter.
STRING1: db "[KayriX86 Bootloader]", 0    ; Reminder: 0xA is newline, 0xD is carriage return
DISK_TEST_MSG: db "DSK_RD_TST: ", 0     ; Disk read test. 
                                        ; We don't have that much space in the boot sector (512 bytes)
                                        ; so its shortened to the minimum
DISK_ID_MSG: db "DSK_ID: ", 0

; Data storage
BOOT_DRIVE: db 0        ; We need to store the index of the drive we booted from

; Padding and magic BIOS number.
times 510-($-$$) db 0
dw 0xaa55

; BOOTSECTOR ENDED HERE

; Writing two test bytes right after the boot sector for disk read test
TEST_HEX: dw ":p"