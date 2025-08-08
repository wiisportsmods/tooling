#ifndef _LIB_COMPRESSION_YAZ0_TYPES_HH
#define _LIB_COMPRESSION_YAZ0_TYPES_HH

#include <inttypes.h>

struct Header {
  char magic[4];
  uint32_t uncompressed_size;
  uint32_t reserved[2];
} __attribute__((packed));

#endif /* _LIB_COMPRESSION_YAZ0_TYPES_HH */