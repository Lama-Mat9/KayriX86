;
;   Loads the kernel at KERNEL_ADDRESS in memory, such that it can be jumped to.
;

KERNEL_ADDRESS equ 0x1000

load_kernel:
    
    mov bx, KERNEL_LOADING_MSG
    call prints
    call newline

    ; ---- Fetching and copying the boot sector ----
    mov al, 15              ; Load 15 sectors
    mov dl, [BOOT_DRIVE]    ; From the drive we just booted from
    mov dh, 0               ; Using head 0
    mov cl, 0x03            ; Starting from the first sector after the two sectors reserved for Bootsector
    mov ch, 0x00            ; With head on track 0
    mov bx, KERNEL_ADDRESS  ; And copy result to the defined address
    call read_dsk

    mov bx, KERNEL_LOADED_MSG
    call prints
    call newline

    ret

; Global data
KERNEL_LOADING_MSG db "Loading kernel...", 0
KERNEL_LOADED_MSG db "Loaded kernel", 0