; Used to read data from disk
read_dsk:

; Parameters

    ; AL: The number of sectors we want to read
    ; DH: The head number we want to read from
    ; DL: The drive number we want to read from
    ; CL: Sector we want to start reading from
    ; CH: Cylinder we want to read from
    ; ES: Destination segment address
    ; BX: Destination offset from destination segment start [ES:BX]  

; Usage example:

    ;mov al, 0x02           ; Load two sectors
    ;mov dl, 0              ; From drive 0
    ;mov dh, 0              ; Using the disk's first head
    ;mov cl, 0x02           ; Starting from the first sector that isnt the boot sector (BootSector is at 0x01)
    ;mov ch, 0x00           ; With head on track 0
    ;mov bx, 0x9000         ; And copy result to ram at this address
    ;call read_dsk

; ----------------------------------------------------------------

    push ax         ; Keep ax on the stack so that we can recall
                    ; how many sectors were expected to be read
                    ; in AL

    mov ah, 0x02    ; BIOS read sector routine code
    int 0x13        ; BIOS sys interrupt

    jc .onError     ; If there was a problem during the read
                    ; The CF (Carry Flag) should be set by the BIOS


    pop dx          ; Restore AL inside of DL
    cmp al, dl      ; This way we compare how many sectors were read (AL)
                    ; compared to how many we wanted to read (DL)
    jne .onError    ; AL != DL      Raises an error


    ret             ; Else we return to caller fine

    ; Only accessed on read error
    .onError:
    
    ; Prints disk error message
    mov bx, DISK_ERROR_MSG      
    call prints

    jmp $           ; Infinite loop


; Define global data
DISK_ERROR_MSG: db "Disk read error !", 0xA, 0xD, 0