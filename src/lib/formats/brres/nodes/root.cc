#include "root.hh"

const std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<node>>>> root::children() const {
  return {std::cref(_children)};
}