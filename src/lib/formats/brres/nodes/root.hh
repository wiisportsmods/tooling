#ifndef _LIB_FORMATS_BRRES_NODES_ROOT_HH
#define _LIB_FORMATS_BRRES_NODES_ROOT_HH

#include "node.hh"

class root : public node {
  const std::vector<std::reference_wrapper<node>> _children;
  
public:
  root(
    const std::vector<std::reference_wrapper<node>> children
  ) : _children(children) {}

  const std::string& name() const override {
    static const std::string name = "(root)";
    return name;
  }

  const std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<node>>>> children() const override;
};

#endif /* _LIB_FORMATS_BRRES_NODES_ROOT_HH */