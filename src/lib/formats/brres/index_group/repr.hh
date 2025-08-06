#ifndef _LIB_FORMATS_BRRES_INDEX_GROUP_REPR_HH
#define _LIB_FORMATS_BRRES_INDEX_GROUP_REPR_HH

#include <string>

#include "lib/bytes/typed_reader.hh"

#include "nodes/node.hh"

const std::string repr(const node& root, const typed_reader& reader);
const std::string repr(const node& root);

#endif /* _LIB_FORMATS_BRRES_INDEX_GROUP_REPR_HH */