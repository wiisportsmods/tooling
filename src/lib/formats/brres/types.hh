#ifndef _LIB_FORMATS_BRRES_TYPES_HH
#define _LIB_FORMATS_BRRES_TYPES_HH

#include <stdlib.h>

/**
 * The header of a `BRRES` file.
 */
struct Header {
  char magic[4];
  uint16_t bom;
  uint16_t padding;
  uint32_t len;
  uint16_t root_offset;
  uint16_t num_sections;
};

/**
 * The header of the `root` section of a `BRRES` file.
 */
struct RootSectionHeader {
  char magic[4];
  uint32_t byte_length;
};

#endif /* _LIB_FORMATS_BRRES_TYPES_HH */