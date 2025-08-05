#ifndef _LIB_FORMATS_BRRES_PARSER_HH
#define _LIB_FORMATS_BRRES_PARSER_HH

#include <list>

#include "lib/bytes/typed_reader.hh"


#include "nodes/data.hh"
#include "nodes/folder.hh"
#include "nodes/root.hh"

#include "string_table.hh"

class parser {
  const typed_reader& _reader;

  std::list<folder> _folders;
  std::list<data> _files;

private:
  template <typename TParent>
  std::vector<std::reference_wrapper<node>> consume_internal(size_t offset);

public:
  parser(
    const typed_reader& reader
  ) : _reader(reader) {}

  root consume(const size_t base_offset);

  const std::list<data>& files() const;
  const std::list<folder>& folders() const;
};


#endif /* _LIB_FORMATS_BRRES_PARSER_HH */