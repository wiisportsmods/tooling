#ifndef _LIB_BYTES_ARRAY_STRUCT_HH
#define _LIB_BYTES_ARRAY_STRUCT_HH

#include "reader.hh"

template <typename T>
class struct_reader;

template <typename T>
class struct_array {
  static_assert(std::is_class<T>::value, "Must be a struct");

  byte_reader& _reader;
  const size_t _base_offset;

public:
  struct_array(
    byte_reader& reader,
    const size_t base_offset
  ) : _reader(reader), _base_offset(base_offset) {}

  struct_reader<T> at(const size_t index) const {
    const size_t offset = _base_offset + (index * sizeof(T));

    struct_reader<T> reader(_reader, offset);
    return reader;
  }
};

#endif /* _LIB_BYTES_ARRAY_STRUCT_HH */