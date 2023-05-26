#include <stdint.h>

#ifndef GENERICADDRESSSTRUCTURE_H
#define GENERICADDRESSSTRUCTURE_H

struct GenericAddressStructure {
  uint8_t AddressSpace;
  uint8_t BitWidth;
  uint8_t BitOffset;
  uint8_t AccessSize;
  uint64_t Address;
} __attribute__ ((packed));

#endif