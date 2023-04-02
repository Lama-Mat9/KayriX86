;
;   Provides a way to switch to protected mode
;

; Switches the CPU to 32 bit protected mode
switch_pm:

; Note:

    ; This function is meant to be called from real mode.
    ; This function does not return control to the caller

; ---------------------------------------------------------------------------------------------------

    ; ---- SWITCHING TO PROTECTED MODE ----:
    cli     ; Clear interrupt: Disable interrupts until they are enabled again.
            ; It is necessary to do since interrups are very different in protected mode.
    
    lgdt [gdt_descriptor]   ; Load GDT Descriptor into GDTR

    ; We must set the first bit of cr0 to 1.
    mov eax, cr0    ; Store cr0 in eax so that we don't remove its current value
    or eax, 0x1     ; Set the first bit of eax to 1
    mov cr0, eax    ; Give back it's value to cr0 but with first bit set to 1
    ; After the CPU finishes updating cr0, we are in protected mode

    ; There is a risk (because of pipelining) that some instructions from real mode 
    ; are not yet finished even though the CPU is now in protected mode.
    ; We need to force the CPU to flush the pipeline. 
    ; This way, no real mode routine or something could be executed in our new protected mode

    ; As the CPU does not know what instuctions it will have to execute after jumping
    ; in the case of a far jump, it flushes the pipeline.
    ; Knowing this, we can use a far jump to flush the pipeline.


    jmp CODE_SEG:flush_pipeline ; Flush the pipeline using a far jump

bits 32 ; We have officially switched to protected mode
        ; so we need to tell NASM so that it generates 32 bit code now

; Nothing particular. Just a label meant so that we can far jump to flush the CPU pipeline.
flush_pipeline:

    mov ebp, 0x90000    ; Update stack base
    mov esp, ebp        ; Empty stack by setting stack pointer to stack base

    ; Our real mode registers have now meaningless values.
    ; We have update them with the appropriate values
    mov ax, DATA_SEG    ; DATA segment index in the GDT is put into ax
    mov ds, ax          ; Properly set data segment
    mov ss, ax          ; Starting address of the stack set to DATA segment
    mov es, ax          ; Additional segments are set to DATA too
    mov fs, ax          ; Additional segments are set to DATA too
    mov gs, ax          ; Additional segments are set to DATA too

    jmp main32          ; Switch to protected mode's main function

%include "src/GDT.asm"
%include "src/x86/main32.asm"