#ifndef _LIB_FORMATS_U8_TYPES_HH
#define _LIB_FORMATS_U8_TYPES_HH

#include <inttypes.h>

struct Header {
  uint32_t magic;
  uint32_t root_offset;
  uint32_t header_size;
  uint32_t data_offset;
  uint8_t _padding[16];
};

struct Node {
  uint16_t type;
  uint16_t name_offset;
  uint32_t data_offset;
  uint32_t size;
} __attribute__((packed));

#endif /* _LIB_FORMATS_U8_TYPES_HH */