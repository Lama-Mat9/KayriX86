; 
;   Kernel entry point code.
;

; Basically the kernel has a main function that needs to be called first.

; The problem is: The bootsector has to jump to a specific addess to begin executing the kernel
; which is the address to where it loaded the kernel.

; In all cases, the bootsector, not knowing where the kernel's main function is,
; will jump to the first instruction that it loaded of the kernel code.

; As we know, the main function is not necessarly compiled as the first instructions in the
; output file from the compiler. This means that the bootsector would jump to any function that
; the C compiler decided to place first in the kernel output code. Not necessarly main.

; We need to ensure that the first instructions in the kernel code are a jump to the kernel's
; main function. And we make it in assembly so that we're certain on how it's gonna come out.
; This is what this code is for. 
; It is meant to be at the start of the kernel code, calling the kernel's main function.

; This way, when the bootsector jumps to the kernel code, it executes this code, which executes the
; kernel's main function

bits 32     ; Tell NASM that we want to generate 32 bit code. 
            ; We should be in PM already much before executing the kernel.
 
extern main ; The kernel's main function is defined in C somewhere else, not here.
            ; This means that we need to tell NASM that the main function exists,
            ; so that we use it, even though it's address is unknown yet.
            ; It will be the linker's role to resolve the call to main as an 
            ; address in the ouput machine code, because the linker will have access
            ; to this code and the kernel's.

call main   ; Make a return-able jump to the kernel's main function.

jmp $       ; If the kernel ever returns to here we stop execution
            ; with this infinite loop (jump to current line).