#include <stdint.h>

#ifndef RSDP_H
#define RSDP_H

struct RSDPDescriptor {
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;
} __attribute__ ((packed));

//Provide to all source files the local RSDP structure
extern struct RSDPDescriptor* RSDP;

uint8_t RSDP_getACPIRevision();
char* RSDP_getOEMID();
int RSDP_init();
int RSDP_isValid(struct RSDPDescriptor* RSDP_addr);

#endif