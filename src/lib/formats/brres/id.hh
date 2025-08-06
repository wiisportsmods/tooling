#ifndef _LIB_FORMATS_BRRES_ID_HH
#define _LIB_FORMATS_BRRES_ID_HH

#include <inttypes.h>
#include <string_view>

/**
 * Calculates the `id` of an index group `node`, by comparing 
 * strings to produce a binary tree.
 */
const uint16_t get_brres_id(
  std::string_view& object,
  std::string_view& subject
);

#endif /* _LIB_FORMATS_BRRES_ID_HH */