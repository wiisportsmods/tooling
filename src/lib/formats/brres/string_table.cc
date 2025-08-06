#include "absl/log/check.h"
#include "absl/log/log.h"

#include "string_table.hh"
#include <sstream>

std::string string_table::copy(const int32_t offset) {
  // The layout of memory in the string table is as follows.
  //
  // `...[N:uint32_t][N*char]...`
  //                  ^ (offset indexes to)
  //
  // By moving the provided offset back by `sizeof(uint32_t)` (4 bytes), we
  // can read the number of characters that we need to read from memory to
  // get the complete string. Strings in this file are *not* null-terminated.
  //
  // String offsets always point to the first `char` of the string.
  const uint32_t size = _reader.read<uint32_t>(_base_offset + offset - sizeof(uint32_t));
  basic_array<char> arr = _reader.read<char[]>(_base_offset + offset);

  // Pre-allocate an `ostringstream` of size `size`.
  std::ostringstream stream(std::string(size, '\0'));

  for(uint32_t i = 0; i < size; i++) {
    stream << arr.at(i);
  }

  return stream.str();
}