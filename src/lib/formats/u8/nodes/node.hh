#ifndef _LIB_FORMATS_U8_NODES_NODE_HH
#define _LIB_FORMATS_U8_NODES_NODE_HH

#include <string>

class node {
  const std::string _name;

protected:  
  node(const std::string name) : _name(name) {}

public:
  virtual ~node() {};

  const std::string& name() const;
};

#endif /* _LIB_FORMATS_U8_NODES_NODE_HH */