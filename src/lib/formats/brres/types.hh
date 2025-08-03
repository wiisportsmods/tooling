#ifndef _LIB_FORMATS_BRRES_TYPES_HH
#define _LIB_FORMATS_BRRES_TYPES_HH

#include <stdlib.h>

struct Header {
  char magic[4];
  uint16_t bom;
  uint16_t padding;
  uint32_t len;
  uint16_t root_offset;
  uint16_t num_sections;
};

struct RootSectionHeader {
  char magic[4];
  uint32_t byte_length;
};

struct IndexGroupHeader {
  uint32_t byte_length;
  uint32_t members;
};

struct IndexGroup {
  uint16_t entry_id;
  uint16_t flag;
  uint16_t left_index;
  uint16_t right_index;
  int32_t name_ptr;
  int32_t data_ptr;
};

static_assert(sizeof(IndexGroup) == 0x10);

#endif /* _LIB_FORMATS_BRRES_TYPES_HH */