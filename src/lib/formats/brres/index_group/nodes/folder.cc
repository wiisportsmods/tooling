#include "folder.hh"

const std::string& folder::name() const {
  return _name;
}

const std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<node>>>> folder::children() const {
  return {std::cref(_children)};
}