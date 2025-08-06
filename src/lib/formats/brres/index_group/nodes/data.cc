#include "data.hh"

const std::string& data::name() const {
  return _name;
}

const size_t data::offset() {
  return _offset;
}