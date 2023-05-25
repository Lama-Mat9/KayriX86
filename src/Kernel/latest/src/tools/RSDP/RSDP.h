#include <stdint.h>

#ifndef RSDP_H
#define RSDP_H

char* RSDP_getAddress();
uint8_t RSDP_getACPIRevision();
char* RSDP_getOEMID();

#endif