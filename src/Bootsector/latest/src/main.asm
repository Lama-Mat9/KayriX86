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

    mov bp, 0x9000      ; Put our stack frame far away from 0x7c00 (this code) so that we don't touch it accidentally
    mov sp, bp          ; Update SP too because we moved the stack frame

    ; ---- Collecting info ----
    mov [BOOT_DRIVE], dl    ; The BIOS should put our boot drive index in DL
                            ; We store now for later before someone erases it

    xor ax, ax              ; Often, the address of the EBDA is stored at 0x040E, 
    mov ah, 0xC1            ; and can also be accessed with int 12, AH=0xC1. However, it is not perfectly certain
    int 0x15                ; that the address is at 0x040E. So we ensure that it is by writing the return value
    mov [0x040e], es        ; of the interrupt over it. The kernel uses it later.
    xor ax, ax
    mov es, ax              ; Reset ES

    ; ---- Setup the display mode ----
    mov al, 0x03        ; 80 x 25 text tty. 16 Colors supported. SHOULD be 720x400 resolution but i'm not sure.
    mov ah, 0x0         ; "Set video mode" BIOS routine parameter
    int 0x10            ; "Video services" BIOS routine interrupt call "code"
    ; Note that it will also clear the screen for us. 
    ; For information about this interrupt, visit: https://en.wikipedia.org/wiki/INT_10H

    ; ---- Boot message test ----
    mov bx, START_STRING ; Put the address of our string inside BX
    call prints     ; Print the string
    call newline    ; Go down one line and carriage return too


    ; ---- Load rest of the code from disk ----
    mov bx, DRIVE_ID_MSG        ; Message that comes with drive ID print
    call prints                 ; It prints the ID that the BIOS gave to our the boot drive

    mov dx, [BOOT_DRIVE]    ; Print the ID that the BIOS gave to our boot drive.
    call printh             ; Typically 0x80 for "first hard disk" on systems that support floppys
    call newline            ; and 0x00 for systems that don't support them.

    mov bx, SIGNATURE_MSG   ; Message that comes with drive read
    call prints

    mov al, 0x1             ; Load one sector
    mov dl, [BOOT_DRIVE]    ; From the drive we just booted from
    mov dh, 0               ; Using the disk's first head
    mov cl, 0x02            ; Starting from the first sector that isnt the boot sector (BootSector is at 0x01)
    mov ch, 0x00            ; With head on track 0
    mov bx, 0x7e00          ; And copy result to ram at this address
    call read_dsk

    mov dx, [TEST_HEX]      ; Retrieve the two last bytes at the sector where we just wrote data to (signature)
    cmp dx, 0x703A          ; Compare these bytes with the expected signature
    je .signature_ok        ; We skip the following if the read signature corresponds to the expected signature
    
    mov bx, SIGNATURE_BAD   ; Tell the user that the signature of loaded sector is not ok
    call prints
    jmp $                   ; Jump to this line (infinite loop, stops execution)

    .signature_ok:          ; If the loaded sector signature was correct
    mov bx, SIGNATURE_OK    ; Tell the user, on the same line as before
    call prints
    call newline

    ; ---- Load the kernel's code ----
    call load_kernel

    ; Quit real mode to use 32 bit protected mode.
    ; Execution will never come back from this jump.
    jmp switch_pm

; -----------------------------------------------------------------------------------------------------

; Global data defined here

; Strings that we could print, 0 string end delimiter.
START_STRING: db "[KayriX86 Bootloader]", 0      ; Reminder: 0xA is newline, 0xD is carriage return

SIGNATURE_MSG: db "Loaded signature: ", 0   ; Disk loaded sector signature test. 
SIGNATURE_OK: db "OK", 0                    ; What's printed when correct signature read
SIGNATURE_BAD: db "Bad signature", 0        ; Printed when bad signature read

DRIVE_ID_MSG: db "Drive ID: ", 0

; Data storage
BOOT_DRIVE: dw 0x0        ; We need to store the index of the drive we booted from

; Includes code here
%include "src/x86_16/print.asm"
%include "src/x86_16/read_dsk.asm"
%include "src/x86_16/load_kernel.asm"

; Padding and magic BIOS number.
times 510-($-$$) db 0   ; Leave 0s everywhere until the 510th byte where we'll write magic number.
dw 0xaa55               ; Magic number indicating to the BIOS that this code is bootable.

; ---- BOOTSECTOR's FIRST SECTOR ENDED HERE ----

%include "src/switch_pm.asm"