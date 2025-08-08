#include "string_table.hh"

std::string string_table::copy(const int32_t offset) const {
  size_t i = 0;
  std::ostringstream stream;

  while(true) {
    char c = _reader.read<char>(_base_offset + offset + i);

    if (c == '\0') {
      break;
    }

    stream << c;
    i++;
  }

  return stream.str();
}