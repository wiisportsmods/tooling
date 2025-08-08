#ifndef _LIB_FORMATS_U8_NODES_FOLDER_HH
#define _LIB_FORMATS_U8_NODES_FOLDER_HH

#include "node.hh"

#include <vector>

class folder : public node {
  const std::vector<std::reference_wrapper<node>> _children;

public:
  static const uint16_t TYPE;

  folder(
    const std::string name,
    const std::vector<std::reference_wrapper<node>> children
  ) : node(name), _children(children) {}

  const std::vector<std::reference_wrapper<node>>& children() const;
};

#endif /* _LIB_FORMATS_U8_NODES_FOLDER_HH */