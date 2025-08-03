#include "absl/log/check.h"
#include "absl/log/log.h"

#include "string_table.hh"

std::string string_table::copy(const int32_t offset) {
  const uint32_t size = _reader.read<uint32_t>(_base_offset + offset - sizeof(uint32_t));
  basic_array<char> arr = _reader.read<char[]>(_base_offset + offset);

  // TODO: string_view this instead? Relies on a `ptr` method from 
  char* buffer = new char[size+1];
  
  CHECK(buffer != nullptr);
  buffer[size] = '\0';

  for(uint32_t i = 0; i < size; i++) {
    buffer[i] = arr.at(i);
  }

  std::string copy(buffer);
  delete[] buffer;

  return copy;
}