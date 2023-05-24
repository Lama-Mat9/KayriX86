;
;   Loads the kernel at KERNEL_ADDRESS in memory, such that it can be jumped to.
;   DOES NOT JUMP TO THE KERNEL.
;

; Kernel code will be copied to this address
KERNEL_ADDRESS equ 0x1000
; Should be right in some free space before the bootsector code.

load_kernel:

    pusha   ; Preserve all registers
    
    ; ---- Print a kernel loading message ----
    mov bx, KERNEL_LOADING_MSG
    call prints
    call newline

    ; ---- Fetching and copying the boot sector ----
    mov al, 30              ; Load 30 sectors (Adjust with kernel code's size)
    mov dl, [BOOT_DRIVE]    ; From the drive we just booted from
    mov dh, 0               ; Using head 0
    mov cl, 0x03            ; Starting from the first sector after the two sectors reserved for Bootsector
    mov ch, 0x00            ; With head on track 0
    mov bx, KERNEL_ADDRESS  ; And copy result to the defined address
    call read_dsk

    ; WARN: Asking QEMU to load more sectors from the disk image than the image contains is fine,
    ; but only as long as we don't ask for too much.

    ; ---- Print a kernel loaded message ----
    mov bx, KERNEL_LOADED_MSG
    call prints
    call newline

    popa    ; Restore all registers

    ret

; Global data
KERNEL_LOADING_MSG db "Loading kernel...", 0
KERNEL_LOADED_MSG db "Loaded kernel", 0
