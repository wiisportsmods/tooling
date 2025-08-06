#include "folder.hh"

const std::string& folder::name() const {
  return _name;
}

const std::vector<std::reference_wrapper<node>>& folder::children() const {
  return _children;
}