#include "file.hh"

const uint16_t file::TYPE = 0x0000;

const size_t file::offset() const {
  return _offset;
}


const size_t file::size() const {
  return _size;
}