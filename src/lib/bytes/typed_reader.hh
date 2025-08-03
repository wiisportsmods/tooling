#ifndef _LIB_BYTES_TYPED_READER_HH
#define _LIB_BYTES_TYPED_READER_HH

#include "reader.hh"
#include "struct_reader.hh"
#include "array/basic.hh"
#include "array/struct.hh"

#include <type_traits>

class typed_reader {
  byte_reader& _reader;

public:
  typed_reader(byte_reader& reader) : _reader(reader) {}

  template <
    typename T,
    typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0
  >
  T read(const size_t position) const {
    return _reader.read<T>(position);
  }

  template <
    typename T,
    typename std::enable_if<std::is_class<T>::value, int>::type = 0
  >
  struct_reader<T> read(const size_t position) const {
    struct_reader<T> reader(_reader, position);
    return reader;
  }


  template <typename T,
    typename std::enable_if<
        std::is_array<T>::value &&
        std::is_arithmetic<typename std::remove_extent<T>::type>::value,
        int>::type = 0>
  basic_array<typename std::remove_extent<T>::type> read(const size_t position) const {
    static_assert(std::is_array<T>::value, "M must be an array");
    static_assert(std::is_arithmetic<typename std::remove_extent<T>::type>::value, "M of M[] must be arithmetic");
  
    basic_array<typename std::remove_extent<T>::type> reader(_reader, position);
    return reader;
  }


  template <typename T,
    typename std::enable_if<
        std::is_array<T>::value &&
        std::is_class<typename std::remove_extent<T>::type>::value,
        int>::type = 0>
  struct_array<typename std::remove_extent<T>::type> read(const size_t position) const {
    static_assert(std::is_array<T>::value, "M must be an array");
    static_assert(std::is_class<typename std::remove_extent<T>::type>::value, "M of M[] must be a struct");
  
    struct_array<typename std::remove_extent<T>::type> reader(_reader, position);
    return reader;
  }
};

#endif /* _LIB_BYTES_TYPED_READER_HH */