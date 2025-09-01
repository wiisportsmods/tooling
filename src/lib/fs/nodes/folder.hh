#ifndef _LIB_FS_NODES_FOLDER_HH
#define _LIB_FS_NODES_FOLDER_HH

#include <vector>
#include <memory>

#include "node.hh"

/**
 * Represents a folder, containing at least one child node.
 */
class folder : public node {
  const std::vector<std::unique_ptr<node>> _children;

public:
  folder(
    const std::string name,
    std::vector<std::unique_ptr<node>> children
  )
    : node(name), _children(std::move(children)) {}

  /**
   * Returns a list of `node`s that are direct children of this `folder`.
   * May contain `data`s, `folder`s, or a mix of both.
   */
  const std::vector<std::unique_ptr<node>>& children() const;
};

#endif /* _LIB_FS_NODES_FOLDER_HH */