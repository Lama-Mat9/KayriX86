;
;   KayriX86 boot sector program (Version 10)
;

org 0x7c00 ; Translate relative addresses to absolute ones
bits 16     ; We're using real mode in the boot sector

boot:   ; Program is read from top to bottom, so this will be the "entry point".
    jmp main    ; We directly jump to main as the rest are just commands for nasm to make a Bios Parameter Block (BPB)
    
    times 0x3E-($-$$) db 0  ; Empty BPB (DOS 4.0 EBPB). Space seems to be allocated until 0x3E. We keep it fully empty.

main:
    xor ax, ax      ; Set AX to 0 so that we can set other registers to 0
    mov ds, ax      ; Set DS to 0
    mov es, ax      ; Set ES to 0

    mov [BOOT_DRIVE], dl    ; The BIOS should put our boot drive index in DL
                            ; We store it for later

    mov bp, 0x8000      ; Put our stack base far away so that we don't touch it accidentally
    mov sp, bp          ; Update SP too because we moved the stack


    ; Welcome message test
    mov bx, STRING1 ; Put the address of our string inside BX
    call prints     ; Print the string


    ; Print data from disk test
    mov al, 0x02            ; Load two sectors
    mov dl, [BOOT_DRIVE]    ; From the drive we just booted from
    mov dh, 0               ; Using the disk's first head
    mov cl, 0x02            ; Starting from the first sector that isnt the boot sector (BootSector is at 0x01)
    mov ch, 0x00            ; With head on track 0
    mov bx, 0x9000          ; And copy result to ram at this address
    call read_dsk

    mov dx, [0x9000]        ; Retrieve the two first bytes at the first sector where we just wrote data
    call printh             ; And print these bytes

    mov dx, [0x9000 + 512]  ; Do the same thing with the next sector we wrote data to
    call printh


    jmp $           ; Jump forever. Program stops there


; Includes code here
%include "src/print.asm"
%include "src/read_dsk.asm"
%include "src/x86/print32.asm"


; Global data defined here

; Strings that we could print, 0 string end delimiter.
STRING1: db "Starting KayriX86 OS... ", 0xA, 0xD, 0            ; Reminder: 0xA is newline
DISK_ERROR_MSG: db "Disk read error !", 0xA, 0xD, 0         ; 0xD is carriage return

; Data storage
BOOT_DRIVE: db 0        ; We need to store the index of the drive we booted from


; Padding and magic BIOS number.
times 510-($-$$) db 0
dw 0xaa55


; BOOTSECTOR ENDED HERE


; Test data to try reading from disk, written far from the bootsector
times 256 dw 0xC0FF     ; 256 words = 512 bytes = First sector
times 256 dw 0xEEEE     ; 256 words = 512 bytes = Second sector