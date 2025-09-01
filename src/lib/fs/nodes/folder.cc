#include "folder.hh"

const std::vector<std::unique_ptr<node>>& folder::children() const {
  return _children;
}