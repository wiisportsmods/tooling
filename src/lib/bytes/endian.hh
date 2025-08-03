#ifndef _LIB_BYTES_ENDIAN_H
#define _LIB_BYTES_ENDIAN_H

#include <type_traits>

enum class Endianness {
  LE,
  BE
};

Endianness get_endianness_from_system();

static inline bool _should_swap(Endianness desired) {
  return desired != get_endianness_from_system();
}

template <typename T>
T as_correct_endianness(
  Endianness desired,
  T value
) {
  if (!_should_swap(desired)) {
    return value;
  }

  union {
    T ty;
    unsigned char bytes[sizeof(T)];
  } source, dest;

  source.ty = value;

  for(size_t k = 0; k < sizeof(T); k++) {
    dest.bytes[k] = source.bytes[sizeof(T) - k - 1];
  }

  return dest.ty;
}

#endif /* _LIB_BYTES_ENDIAN_H */