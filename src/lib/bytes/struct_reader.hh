#ifndef _LIB_BYTES_STRUCT_READER_H
#define _LIB_BYTES_STRUCT_READER_H

#include "reader.hh"

#include "array/basic.hh"
#include "array/struct.hh"

#include <stdlib.h>

template <typename T>
class struct_reader {
  byte_reader& _reader;
  size_t _base_offset;

public:
  struct_reader(
    byte_reader& reader,
    size_t base_offset
  ) : _reader(reader), _base_offset(base_offset) {};

  const inline size_t offset() {
    return _base_offset;
  }

  template <
    typename M,
    typename std::enable_if<
        std::is_array<M>::value &&
        std::is_arithmetic<typename std::remove_extent<M>::type>::value,
        int>::type = 0
  >
  basic_array<typename std::remove_extent<M>::type> get(
    M T::*member
  ) const {
    T* dummy = nullptr;
    size_t member_offset = reinterpret_cast<size_t>(&(dummy->*member));
    
    basic_array<typename std::remove_extent<M>::type> reader(
      _reader,
      _base_offset + member_offset
    );
    
    return reader;
  }

  template <
    typename M,
    typename std::enable_if<
        std::is_array<M>::value &&
        std::is_class<typename std::remove_extent<M>::type>::value,
        int>::type = 0
  >
  struct_array<typename std::remove_extent<M>::type> get(
    M T::*member
  ) const {
    T* dummy = nullptr;
    size_t member_offset = reinterpret_cast<size_t>(&(dummy->*member));
    

    struct_array<typename std::remove_extent<M>::type> reader(
      _reader,
      _base_offset + member_offset
    );
    
    return reader;
  }

  template <
    typename M,
    typename std::enable_if<
        std::is_class<M>::value,
        int>::type = 0
  >
  struct_reader<M> get(
    M T::*member
  ) const {
    T* dummy = nullptr;
    size_t member_offset = reinterpret_cast<size_t>(&(dummy->*member));
    
    struct_reader<M> reader(
      _reader,
      _base_offset + member_offset
    );

    return reader;
  }

  template <typename M, typename std::enable_if<std::is_arithmetic<M>::value, int>::type = 0>
  M get(
    M T::*member
  ) const {
    T* dummy = nullptr;
    size_t member_offset = reinterpret_cast<size_t>(&(dummy->*member));

    return _reader.read<M>(_base_offset + member_offset);
  }
};

#endif /* _LIB_BYTES_STRUCT_READER_H */