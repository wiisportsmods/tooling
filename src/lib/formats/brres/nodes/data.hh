#ifndef _LIB_FORMATS_BRRES_DATA_HH
#define _LIB_FORMATS_BRRES_DATA_HH

#include <stdlib.h>
#include <string>

#include "node.hh"

class data : public node {
  const std::string _name;
  const size_t _offset;

public:
  data(
    const std::string name,
    const size_t offset
  ) : _name(name), _offset(offset) {}

  const std::string& name() const override;
  const std::optional<std::reference_wrapper<const std::vector<std::reference_wrapper<node>>>> children() const override;
  const size_t offset();
};

#endif /* _LIB_FORMATS_BRRES_DATA_HH */