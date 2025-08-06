#ifndef _LIB_FORMATS_BRRES_INDEX_GROUP_TYPES_HH
#define _LIB_FORMATS_BRRES_INDEX_GROUP_TYPES_HH

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

#endif /* _LIB_FORMATS_BRRES_INDEX_GROUP_TYPES_HH */