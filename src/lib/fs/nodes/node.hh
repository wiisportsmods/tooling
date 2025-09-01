#ifndef _LIB_FS_NODE_HH
#define _LIB_FS_NODE_HH

#include <string>

/**
 * Represents a `node` in the file system, which has a name.
 * 
 * To get more specific information, cast this to a concrete type
 * using `dynamic_cast`.
 * 
 * @interface
 */
class node {
  const std::string _name;

public:
  node(const std::string name) : _name(name) {}
  virtual ~node() = default;

  /**
   * Returns the name of this node. All nodes have a name. The root node 
   * is given a special name, `(root)`, identifying it as the root node.
   * 
   * Do not rely on the `(root)` name to deliminate the root node from others.
   */
  const std::string& name() const;
};

#endif /* _LIB_FORMATS_BRRES_NODE_HH */