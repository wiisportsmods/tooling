#include "size.hh"

#include <string>
#include <array>

#include "absl/strings/str_format.h"

static const std::array<std::string, 4> _units {{"b", "kb", "mb", "gb"}};
static constexpr size_t _max_units_index = _units.size() - 1;

const std::string format_byte_size(const size_t byte_length) {
  double size = (double)byte_length;
  size_t index = 0;

  while (size > 1023 && index < _max_units_index) {
    size /= 1024.0f;
    index++;
  }

  return absl::StrFormat(
    "%.3lf%s",
    size,
    _units[index]
  );
}