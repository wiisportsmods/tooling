#ifndef _LIB_BUFFER_BYTE_READER_HH
#define _LIB_BUFFER_BYTE_READER_HH

#include "absl/log/check.h"

#include "endian.hh"

#include <stddef.h>
#include <optional>
#include <span>
#include <memory>

/**
 * A simple reader that can handle `arithmetic` types (based on `std::is_arithmetic`).
 * Handles swapping endianness if a desired endianness is specified.
 * 
 * If you need to handle more complex types (structs, arrays), see `typed_reader`.
 * 
 * Most usage of this is through composition w/ `typed_reader`.
 */
class byte_reader {
  /**
   * The owned buffer to read from.
   */
  std::unique_ptr<char[]> _data;

  /**
   * The length of the buffer.
   */
  const size_t _len;
  
  /**
   * The preferred endianness, if known.
   */
  std::optional<Endianness> _endianness;

public:
  /** 
   * Creates a reader that is unaware of the endianness of the data. 
   * 
   * @param buffer The buffer to read from.
   * @param len The length of the buffer.
   */
  byte_reader(
    std::unique_ptr<char[]> buffer, 
    size_t len
  ) : _data(std::move(buffer)), _len(len) {};

  /**
   * Creates a reader that is aware of the endianness of the data.
   * 
   * @param buffer The buffer to read from.
   * @param len The length of the buffer.
   * @param endianness The endianness to convert values to.
   */
  byte_reader(
    std::unique_ptr<char[]> buffer,
    const size_t len,
    const Endianness endianness
  ) : byte_reader(std::move(buffer), len) {
    _endianness = endianness;
  }

  // No move
  byte_reader&& operator=(byte_reader&& other) = delete;
  byte_reader(byte_reader&& other) = delete;

  // No copy
  byte_reader& operator=(byte_reader& other) = delete;
  byte_reader(const byte_reader& rhs) = delete;

  /**
   * Reads a `T` from the buffer at `position`, provided `T` is an
   * arithmetic value (via `std::is_arithmetic`).
   * 
   * Asserts that there is no out of bounds reading happening.
   */
  template <typename T> 
  T read(const size_t position) const {
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

  /**
   * Returns a span of `len` at `position` of type `T`.
   * 
   * Asserts there is no out of bounds reading happening.
   */
  template <typename T>
  T* span(const size_t position, const size_t len) const {
    const size_t size = sizeof(T);

    CHECK(position + size <= _len)  
      << "Position " << position << "is out of bounds (max:" << _len << ")";


    std::span span(_data.get() + position, len);
    return span;
  }
    
  const size_t length() const;

  void swap_endianness();
};

#endif /* _LIB_BUFFER_BYTE_READER_HH */