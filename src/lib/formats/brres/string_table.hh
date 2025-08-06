#ifndef _LIB_FORMATS_BRRES_STRING_HH
#define _LIB_FORMATS_BRRES_STRING_HH

#include "lib/bytes/typed_reader.hh"

/**
 * A reader, that allows you to copy `string` values from the string table, 
 * which is usually at the end of the `brres` file.
 */
class string_table {
  const typed_reader& _reader;
  const size_t _base_offset;
public:
  /**
   * Creates a reader that will index relative to the start of the file.
   */
  string_table(const typed_reader& reader) : string_table(reader, 0) {}
    
  /**
   * Creates a reader that will index relative to `base_offset`.
   */
  string_table(
    const typed_reader& reader,
    const size_t base_offset
  ) : _reader(reader), _base_offset(base_offset) {}

  /**
   * Copy the string at offset (relative to `base_offset`) in the table as a `std::string`.
   */
  std::string copy(const int32_t offset);
};  

#endif /* _LIB_FORMATS_BRRES_STRING_HH */