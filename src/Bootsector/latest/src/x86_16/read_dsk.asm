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

    mov ah, 0x02    ; BIOS read sector routine code
    int 0x13        ; BIOS sys interrupt

    jc .onError     ; If there was a problem during the read
                    ; The CF (Carry Flag) should be set by the BIOS

    ; Wikipedia's docs about int 13h (for ah=02h) (https://en.wikipedia.org/wiki/INT_13H)
    ; says that the number of sectors read should be returned in AL.
    ; Because of this, i used to check AL (after using the interrupt) corresponded
    ; to the number of sectors that we want. 
    ; It worked on some bioses, but i had to stop doing it because i got an HP bios that
    ; successfully reads data as expected but does not return the number of sectors read.
    ; It ended up triggering the error message even though everything was fine. 

    ret

    ; Only accessed on read error
    .onError:
    
    ; Prints disk error message
    mov bx, DISK_RD_ERROR_MSG      
    call prints

    jmp $           ; Infinite loop


; Define global data
DISK_RD_ERROR_MSG: db "DISK_READ_ERROR!", 0