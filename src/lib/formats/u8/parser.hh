#ifndef _LIB_FORMATS_U8_READER_HH
#define _LIB_FORMATS_U8_READER_HH

#include <list>
#include <unordered_map>

#include "typed_reader.hh"


#include "nodes/node.hh"
#include "nodes/folder.hh"
#include "nodes/file.hh"

#include "string_table.hh"
#include "types.hh"

class parser {
  const typed_reader& _reader;

  std::unique_ptr<node> build_from_reverse_relationships(
    struct_array<Node>& nodes,
    const string_table& table,
    const std::unordered_multimap<size_t, size_t>& reverse_relationships
  );

  std::unique_ptr<node> build_from_reverse_relationships(
    struct_array<Node>& nodes,
    const string_table& table,
    const std::unordered_multimap<size_t, size_t>& reverse_relationships,
    size_t curr
  );

public:
  parser(
    const typed_reader& reader
  ) : _reader(reader) {}

  std::unique_ptr<node> consume();
};

#endif /* _LIB_FORMATS_U8_READER_HH */