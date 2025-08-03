#ifndef _LIB_FORMATS_BRRES_PARSER_HH
#define _LIB_FORMATS_BRRES_PARSER_HH

#include <vector>

#include "lib/bytes/typed_reader.hh"

#include "fs_entry.hh"
#include "string_table.hh"

class parser {
  const typed_reader& _reader;

  // We can iterate over the binary to calculate the size of this
  // to reserve before creation, but for now we don't do this.
  std::vector<fs_entry> _entries;

private:
  void consume_children(
    fs_entry& parent,
    size_t offset
  );

public:
  parser(
    const typed_reader& reader
  ) : _reader(reader) {}

  fs_entry& consume(const size_t base_offset);
};


#endif /* _LIB_FORMATS_BRRES_PARSER_HH */