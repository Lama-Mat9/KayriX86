#include <stdint.h>
#include "tools/ACPI/SDTHeader.h"

#ifndef RSDT_H
#define RSDT_H

struct RSDT {
    struct ACPISDTHeader SDTheader;
    uint32_t* FirstSDT;
};

//Provide to all source files the local RSDT structure
extern struct RSDT* RootSDT;

int RSDT_init();

#endif