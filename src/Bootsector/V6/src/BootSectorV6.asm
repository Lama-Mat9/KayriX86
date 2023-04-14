;
;   Simple boot sector program (Version 6)
;   Fixes the problem that made it almost impossible to run this boot sector on real hardware   
;

org 0x7c00 ; Translate relative addresses to absolute ones
bits 16 ; We're still in 16 bit real mode

boot:   ; Program is read from top to bottom, so this will be the "entry point".
    jmp main    ; We directly jump to main as the rest are just commands for nasm to make a Bios Parameter Block (BPB)
    
    ; In short, some bioses do USB floopy disk drive emulation. 
    ; The Boot Sector has to contain something called BPB, it describes how the physical media is setup.
    ; On emulators, this is unnecessary. On real hardware, bioses tend to seach for it although it's only really necessary for floppy disks.
    ; This means that it should be fine using whatever values inside of this.
    
    TIMES 3-($-$$) db 0x90   ; Support 2 or 3 byte encoded JMPs before BPB.

    ; Dos 4.0 EBPB 1.44MB floppy
    OEMname:           db    "mkfs.fat"  ; mkfs.fat is what OEMname mkdosfs uses
    bytesPerSector:    dw    512    ; Offset: 0x0B. How many bytes make 1 sector
    sectPerCluster:    db    1      ; Offset: 0x0D. How much sectors are allocated for 1 Allocation unit. HPFS uses 1, we do the same.
    reservedSectors:   dw    1      ; Offset: 0x0E. How much sectors are reserved to our boot sector.
    numFAT:            db    2      ; Offset: 0x10. How many copied of the Fille Allocation Table exist. 2 is the convention.
    RootDirSize:       dw    224    ; Offset: 0x11. Size of the root directory. 
    numSectors:        dw    2880   ; Offset: 0x13. How many sectors are available on the volume.
    mediaType:         db    0xf0   ; Offset: 0x15. Describes the physical disk. We don't care because we have an emulated floppy.
    numFATsectors:     dw    9
    sectorsPerTrack:   dw    18
    numHeads:          dw    2
    numHiddenSectors:  dd    0
    numSectorsHuge:    dd    0
    driveNum:          db    0
    reserved:          db    0
    signature:         db    0x29       ; Offset: 0x26. This is a magic number. It indicates that this is a MS-DOS V4.0 BPB
    volumeID:          dd    0x2d7e5a1a ; Offset: 0x27. Unique number that distinguishes this volume from another volume. 
    volumeLabel:       db    "BOOT VOLUME"
    fileSysType:       db    "FAT12"

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