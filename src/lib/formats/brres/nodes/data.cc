#include "data.hh"

const std::string& data::name() const {
  return _name;
}

const std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<node>>>> data::children() const {
  return {};
}

const size_t data::offset() {
  return _offset;
}