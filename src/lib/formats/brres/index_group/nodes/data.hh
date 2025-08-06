#ifndef _LIB_FORMATS_BRRES_DATA_HH
#define _LIB_FORMATS_BRRES_DATA_HH

#include <stdlib.h>
#include <string>

#include "node.hh"

/**
 * Represents a `node` in the BRRES File System (Index Groups)
 * that points to a file, or other blob of data.
 */
class data : public node {
  const std::string _name;
  const size_t _offset;

public:
  data(
    const std::string name,
    const size_t offset
  ) : _name(name), _offset(offset) {}

  /**
   * Returns the name of the node.
   */
  const std::string& name() const override;

  /**
   * Returns the `offset`, relative to the start of the `BRRES` file,
   * that the data node points to. 
   */
  const size_t offset();
};

#endif /* _LIB_FORMATS_BRRES_DATA_HH */