#include "file.hh"

const size_t file::offset() const {
  return _offset;
}

const size_t file::size() const {
  return _size;
}