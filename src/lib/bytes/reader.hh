#ifndef _LIB_BUFFER_BYTE_READER_HH
#define _LIB_BUFFER_BYTE_READER_HH

#include "absl/log/check.h"

#include "endian.hh"

#include <stddef.h>
#include <optional>
#include <span>
#include <memory>

class byte_reader {
  std::unique_ptr<char[]> _data;
  const size_t _len;
  std::optional<Endianness> _endianness;

public:
  byte_reader(
    std::unique_ptr<char[]> buffer, 
    size_t len
  ) : _data(std::move(buffer)), _len(len) {};

  byte_reader(
    std::unique_ptr<char[]> buffer,
    const size_t len,
    const Endianness endianness
  ) : byte_reader(std::move(buffer), len) {
    _endianness = endianness;
  }

  byte_reader&& operator=(byte_reader&& other) = delete;
  byte_reader(byte_reader&& other) = delete;

  byte_reader& operator=(byte_reader& other) = delete;
  byte_reader(const byte_reader& rhs) = delete;

  template <typename T> 
  T read(const size_t position) {
    static_assert(std::is_arithmetic<T>::value, "T must be a fundamental value");

    const size_t size = sizeof(T);

    CHECK(position + size <= _len)  
      << "Position " << position << " is out of bounds (max:" << _len << ")";
      
    T data = *reinterpret_cast<const T*>(
      _data.get() + position
    );

    if (_endianness.has_value()) {
      return as_correct_endianness(
        *_endianness,
        data
      );
    }

    return data;
  }

  template <typename T>
  T* span(const size_t position, const size_t len) {
    const size_t size = sizeof(T);

    CHECK(position + size <= _len)  
      << "Position " << position << "is out of bounds (max:" << _len << ")";


    std::span span(_data.get() + position, len);
    return span;
  }
    
  size_t length();

  void swap_endianness();
};

#endif /* _LIB_BUFFER_BYTE_READER_HH */