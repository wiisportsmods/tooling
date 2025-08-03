#include "fs_entry.hh"

const std::string& fs_entry::name() {
  return _name;
}

std::vector<fs_entry>& fs_entry::children() {
  return _children;
}

const bool fs_entry::root() {
  return _is_root;
}