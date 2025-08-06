#ifndef _LIB_BYTES_ARRAY_BASIC_HH
#define _LIB_BYTES_ARRAY_BASIC_HH

#include "reader.hh"

// TODO: `basic_array` and `struct_array` should have a constructor that supports bounds checking.

template <typename T>
class basic_array {
  static_assert(std::is_arithmetic<T>::value, "Must be arithmetic value");

  byte_reader& _reader;
  const size_t _base_offset;

public:
  /** 
   * Constructs a reader of `T` starting at `base_offset`.
   */
  basic_array(
    byte_reader& reader,
    const size_t base_offset
  ) : _reader(reader), _base_offset(base_offset) {}

  /**
   * Get the element at `index`, relative to `base_offset`.
   * 
   * @returns The value at `index`.
   */
  T at(const size_t index) {
    const size_t offset = _base_offset + (index * sizeof(T));
    return _reader.read<T>(offset);
  }
};

#endif /* _LIB_BYTES_ARRAY_BASIC_HH */