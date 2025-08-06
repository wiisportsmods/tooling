#ifndef _LIB_FORMATS_BRRES_NODE_HH
#define _LIB_FORMATS_BRRES_NODE_HH

#include <list>
#include <optional>

class node {
public:
  virtual ~node() = default;
  
  virtual const std::string& name() const = 0;
  virtual const std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<node>>>> children() const = 0;
};

#endif /* _LIB_FORMATS_BRRES_NODE_HH */