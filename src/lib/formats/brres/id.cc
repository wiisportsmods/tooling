#include "id.hh"

static inline uint16_t get_highest_bit(uint8_t value) {
  uint16_t i;
  for(i = 7; i > 0 && !(value & 0x80); i--, value <<= 1)
    ;

  return value;
}

const uint16_t get_brres_id(
  std::string_view& object,
  std::string_view& subject
) {
  if (object.length() < subject.length()) {
    return subject.length() - 1 << 3 | get_highest_bit(subject[subject.length() - 1]);
  }

  size_t len = subject.length();

  while(len-- > 0) {
    const uint8_t c = object[len] ^ subject[len];

    if (c) {
      return subject.length() << 3 | get_highest_bit(c);
    }
  }

  return ~(uint16_t)0;
}