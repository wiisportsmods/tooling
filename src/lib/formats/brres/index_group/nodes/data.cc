#include "data.hh"

const std::string& data::name() const {
  return _name;
}

const size_t data::offset() const {
  return _offset;
}

const size_t data::size() const {
  return _size;
}