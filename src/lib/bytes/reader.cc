#include "reader.hh"

#include <type_traits>

#include "absl/log/log.h"

void byte_reader::swap_endianness()  {
  if (!_endianness) {
    _endianness = get_endianness_from_system();
  }

  _endianness = *_endianness == Endianness::LE 
    ? Endianness::BE
    : Endianness::LE;
}
