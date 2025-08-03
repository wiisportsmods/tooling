#include "endian.hh"

#include <bit>

Endianness get_endianness_from_system() {
  switch (std::endian::native) {
    case std::endian::big:
      return Endianness::BE;
    case std::endian::little:
      return Endianness::LE;
  }
}