#include "folder.hh"

const uint16_t folder::TYPE = 0x0100;

const std::vector<std::reference_wrapper<node>>& folder::children() const {
  return _children;
}
