#ifndef _LIB_FORMATS_BRRES_HH
#define _LIB_FORMATS_BRRES_HH

#include <optional>
#include <vector>
#include <span>
#include <expected>

#include "node.hh"

class folder : public node {
  const std::string _name;
  const std::vector<std::reference_wrapper<node>> _children;

public:
  folder(
    const std::string name,
    const std::vector<std::reference_wrapper<node>> children
  )
    : _name(name), _children(children) {}

  const std::string& name() const override;
  const std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<node>>>> children() const override;
};

#endif /* _LIB_FORMATS_BRRES_HH */