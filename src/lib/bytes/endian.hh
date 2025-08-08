#ifndef _LIB_BYTES_ENDIAN_H
#define _LIB_BYTES_ENDIAN_H

#include <type_traits>

/**
 * The possible endianness that we support.
 */
enum class Endianness {
  LE,
  BE
};

Endianness get_endianness_from_system();

/**
 * @returns True if there is an endianness mismatch between desired and system.
 */
static inline bool _should_swap(Endianness desired) {
  return desired != get_endianness_from_system();
}

/**
 * Converts `T` to the correct endianness if required.
 */
template <typename T>
T as_correct_endianness(
  Endianness desired,
  T value
) {
  static_assert(sizeof(T) == 1 || sizeof(T) % 2 == 0);
  
  if (!_should_swap(desired) || sizeof(T) == 1) {
    return value;
  }

  union {
    T ty;
    unsigned char bytes[sizeof(T)];
  } source, dest;

  source.ty = value;

  // Swap each byte in pairs moving inwards from each end.
  for(size_t k = 0; k < sizeof(T); k++) {
    dest.bytes[k] = source.bytes[sizeof(T) - k - 1];
  }

  return dest.ty;
}

#endif /* _LIB_BYTES_ENDIAN_H */