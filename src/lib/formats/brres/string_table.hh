#ifndef _LIB_FORMATS_BRRES_STRING_HH
#define _LIB_FORMATS_BRRES_STRING_HH

#include "lib/bytes/typed_reader.hh"

class string_table {
  const typed_reader& _reader;
  const size_t _base_offset;
public:
  string_table(const typed_reader& reader) : string_table(reader, 0) {}
    
  string_table(
    const typed_reader& reader,
    const size_t base_offset
  ) : _reader(reader), _base_offset(base_offset) {}

  std::string copy(const int32_t offset);
};  

#endif /* _LIB_FORMATS_BRRES_STRING_HH */