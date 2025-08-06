#ifndef _LIB_FORMATS_BRRES_HH
#define _LIB_FORMATS_BRRES_HH

#include <optional>
#include <vector>
#include <span>
#include <expected>

#include "node.hh"

/**
 * Represents a folder, containing at least one child node.
 */
class folder : public node {
  const std::string _name;
  const std::vector<std::reference_wrapper<node>> _children;

public:
  folder(
    const std::string name,
    const std::vector<std::reference_wrapper<node>> children
  )
    : _name(name), _children(children) {}

  /**
   * Returns the name of this node.
   */
  const std::string& name() const override;

  /**
   * Returns a list of `node`s that are direct children of this `folder`.
   * May contain `data`s, `folder`s, or a mix of both.
   */
  const std::vector<std::reference_wrapper<node>>& children() const;
};

#endif /* _LIB_FORMATS_BRRES_HH */