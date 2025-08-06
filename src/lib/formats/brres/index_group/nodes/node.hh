#ifndef _LIB_FORMATS_BRRES_NODE_HH
#define _LIB_FORMATS_BRRES_NODE_HH

#include <list>
#include <optional>

/**
 * Represents a `node` in the file system, which can have a name, and children.
 * 
 */
class node {
public:
  virtual ~node() = default;
  virtual const std::string& name() const = 0;
};

#endif /* _LIB_FORMATS_BRRES_NODE_HH */