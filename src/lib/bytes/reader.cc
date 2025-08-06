#include "reader.hh"

#include <type_traits>

#include "absl/log/log.h"

void byte_reader::swap_endianness() {
  // Swap from system to non-system endianness if
  // none is already specified. Useful if reading a `BOM`
  // and swapping based on that.
  if (!_endianness) {
    _endianness = get_endianness_from_system();
  }

  _endianness = *_endianness == Endianness::LE 
    ? Endianness::BE
    : Endianness::LE;
}
