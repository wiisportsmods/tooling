#include "string_table.hh"

std::string string_table::copy(const int32_t offset) const {
  std::ostringstream stream;
  char c;
  size_t i = _base_offset + offset;
  
  while ((c = _reader.read<char>(i++)) != '\0') {
    stream << c;
  }

  return stream.str();
}