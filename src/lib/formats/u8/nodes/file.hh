#ifndef _LIB_FORMATS_U8_NODES_FILE_HH
#define _LIB_FORMATS_U8_NODES_FILE_HH

#include "node.hh"

class file : public node {
  const size_t _offset;
  const size_t _size;

public:
  static const uint16_t TYPE;

  file(
    const std::string name,
    const size_t offset,
    const size_t size
  ) : node(name), _offset(offset), _size(size) {}

  const size_t offset() const;
  const size_t size() const;
};

#endif /* _LIB_FORMATS_U8_NODES_FILE_HH */