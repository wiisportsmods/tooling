#ifndef _LIB_FS_NODES_FILE_HH
#define _LIB_FS_NODES_FILE_HH

#include <cstdlib>
#include <string>

#include "node.hh"

/**
 * Represents a `node` in the BRRES File System (Index Groups)
 * that points to a file, or other blob of data.
 */
class file : public node {
  const std::string _name;
  const size_t _offset;
  const size_t _size;

public:
  file(
    const std::string name,
    const size_t offset,
    const size_t size
  ) : node(name), _offset(offset), _size(size) {}

  /**
   * Returns the `offset`, relative to the reader that was used to read this
   * file, that the node points to. 
   */
  const size_t offset() const;

  /**
   * Returns the `size` of the data the node points to. 
   */
  const size_t size() const;
};

#endif /* _LIB_FS_NODES_FILE_HH */