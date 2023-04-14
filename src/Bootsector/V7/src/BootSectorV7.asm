;
;   Simple boot sector program (Version 7)
;   Emptied BPB to avoid problems later
;

org 0x7c00 ; Translate relative addresses to absolute ones
bits 16 ; We're still in 16 bit real mode

boot:   ; Program is read from top to bottom, so this will be the "entry point".
    jmp main    ; We directly jump to main as the rest are just commands for nasm to make a Bios Parameter Block (BPB)
    
    ; In short: The Boot Sector has to contain something called BPB, it describes how the physical media is setup.
    ; On emulators, this is unnecessary. On real hardware, bioses tend to seach for it although it's only really necessary for floppy disks.
    ; This means that it should be fine using whatever values inside of the BPB. 
    ; Even better: I found out that i can leave it empty. As long as we don't use the space allocated for the BPB it looks like BIOSes accept to run the code correctly.

    times 0x3E-($-$$) db 0  ; Empty BPB (DOS 4.0 EBPB). Space seems to be allocated until 0x3E. We keep it fully empty.

main:
    xor ax, ax      ; Set AX to 0 so that we can set other registers to 0
    mov ds, ax      ; Set DS to 0
    mov es, ax      ; Set ES to 0

    mov ecx, string1 ; Set the reference to our string inside ECX as defined in print.asm
    call print

    mov ecx, string2
    call print

    jmp $ ; Jump forever. Code stops there

; Add print.asm code here
%include "src/print.asm"

; Data is being defined here
string1:    ; Our strings use CRLF line endings, which means that they signify that they have ended when they find CR (0x0A) and LF (0x0D) 
    db "Hello world!"
    dw 0x0D0A ; Note: This is 0x0A0D on disk, but x86 works in little endian [smaller values right, bigger left] which inverts code
string2:
    dw "This boot sector implements a way of printing strings in assembly, which allows me to print long sentences fairly easily."
    dw 0x0D0A ; Note: This is 0x0A0D on disk, but x86 works in little endian [smaller values right, bigger left] which inverts code

; Padding and magic BIOS number.
times 510-($-$$) db 0
dw 0xaa55