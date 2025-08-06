#ifndef _LIB_FORMATS_BRRES_PARSER_HH
#define _LIB_FORMATS_BRRES_PARSER_HH

#include <list>

#include "lib/bytes/typed_reader.hh"
#include "lib/formats/brres/string_table.hh"

#include "nodes/data.hh"
#include "nodes/folder.hh"


class parser {
  const typed_reader& _reader;
  const size_t _base_offset;
  const std::optional<size_t> _length;

  std::list<folder> _folders;
  std::list<data> _files;

private:
  std::vector<std::reference_wrapper<node>> consume_internal(size_t offset);
  bool is_group(size_t offset);

public:
  parser(
    const typed_reader& reader,
    const size_t base_offset,
    const size_t length
  ) : _reader(reader), _base_offset(base_offset), _length(length) {}
  
  parser(
    const typed_reader& reader,
    const size_t base_offset
  ) : _reader(reader), _base_offset(base_offset), _length(std::nullopt) {}

  folder consume();

  const std::list<data>& files() const;
  const std::list<folder>& folders() const;
};


#endif /* _LIB_FORMATS_BRRES_PARSER_HH */