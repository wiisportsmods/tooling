#ifndef _LIB_FORMATS_BRRES_PARSER_HH
#define _LIB_FORMATS_BRRES_PARSER_HH

#include <list>
#include <memory>

#include "lib/bytes/typed_reader.hh"
#include "lib/formats/brres/string_table.hh"

#include "lib/fs/nodes/node.hh"
#include "lib/fs/nodes/folder.hh"

class parser {
  const typed_reader& _reader;
  const size_t _base_offset;
  const std::optional<size_t> _length;

private:
  /**
   * Recursively handles parsing groups, adding folders and files to their
   * containers (`_folders` and `_files` respectively), and then returning references
   * to them.
   * 
   * @param offset The offset this group starts at.
   * @returns A vector of references to nodes, the direct children of the current node.
   */
  std::vector<std::unique_ptr<node>> consume_internal(size_t offset);

  /**
   * Returns true if the relative `offset` (relative to `_base_offset`)
   * refers to another group, or false if it refers to data.
   * 
   * @param offset The offset to the beginning of this group.
   * @returns True if the offset is a group, false otherwise.
   */
  bool is_group(size_t offset);

public:
  /** 
   * Constructs a parser for a complete file system, using the provided
   * `length` to delimitate if a child of a node is a `folder` or `data`
   * entry, at `base_offset`.
   */
  parser(
    const typed_reader& reader,
    const size_t base_offset,
    const size_t length
  ) : _reader(reader), _base_offset(base_offset), _length(length) {}
  
  /**
   * Constructs a parser for a single `folder`, at `base_offset`.
   */
  parser(
    const typed_reader& reader,
    const size_t base_offset
  ) : _reader(reader), _base_offset(base_offset), _length(std::nullopt) {}

  /**
   * Consumes the data at `base_offset`, parsing it to `node`s in a directory
   * structure.
   */
  std::unique_ptr<folder> consume();

  /**
   * Consumes the data at `base_offset`, parsing it to `node`s in a directory
   * structure.
   */
  std::unique_ptr<folder> consume(const std::string root_folder_name);
};


#endif /* _LIB_FORMATS_BRRES_PARSER_HH */