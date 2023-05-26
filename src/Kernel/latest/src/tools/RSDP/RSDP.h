#include <stdint.h>

#ifndef RSDP_H
#define RSDP_H

struct RSDP {
    
    //ACPI >= 1.0
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;

    //ACPI >= 2.0
    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExtendedChecksum;
    uint8_t reserved[3];

} __attribute__ ((packed));

//Provide to all source files the local RSDP structure
extern struct RSDP* RootSDP;

uint8_t RSDP_getACPIRevision();
char* RSDP_getOEMID();
int RSDP_init();

#endif