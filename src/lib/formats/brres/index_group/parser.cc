#include "parser.hh"
#include "types.hh"

#include "lib/fs/nodes/node.hh"
#include "lib/fs/nodes/folder.hh"
#include "lib/fs/nodes/file.hh"

#include "absl/log/log.h"

#include <iostream>

bool parser::is_group(size_t offset) {
  // If there is no known length, then we assume that
  // this is a flat folder w/ no non-data children. This is
  // usually the case for the non-root Index Group.
  if (!_length) {
    return false;
  }

  return offset <= (*_length);
}

std::unique_ptr<folder> parser::consume() {
  // Create a fake folder called "(root)" to act as the root,
  // and then call the recursive implementation to produce the
  // tree structure.
  return consume("(root)");
}

std::unique_ptr<folder> parser::consume(const std::string root_folder_name) {
  return std::make_unique<folder>(
    root_folder_name, 
    consume_internal(_base_offset)
  );
}

std::vector<std::unique_ptr<node>> parser::consume_internal(
  size_t offset
) {
  // Read the header to know how many members to expect.
  struct_reader<IndexGroupHeader> header = _reader.read<IndexGroupHeader>(
    offset
  );

  // Prepare a string reader to be able to read the names of nodes.
  string_table table(_reader, offset);

  // The number of members in this group. Index them using `1..=count`
  uint32_t count = header.get(&IndexGroupHeader::members);

  // The number of bytes this group consists of.
  uint32_t byte_size = header.get(&IndexGroupHeader::byte_length);

  VLOG(4) << "Consuming group" << "\n"
    << "byte_size(" << byte_size << ")" << "\n"
    << "count(" << count << ")" << "\n"
    << "offset(" << offset << ")";

  // Read the groups as an array.
  struct_array<IndexGroup> groups = _reader.read<IndexGroup[]>(
    offset + sizeof(IndexGroupHeader)
  );

  // We know there will be `count` children, so create a vector of
  // `count` `unique_ptr<node>`s ahead of time.
  std::vector<std::unique_ptr<node>> children;
  children.reserve(count);

  // Skip the first one, as that is a special value to provide the binary
  // tree structure (based on `index` values).
  for(uint32_t i = 1; i < count + 1; i++) {
    struct_reader<IndexGroup> group = groups.at(i);

    std::string name = table.copy(
      group.get(&IndexGroup::name_ptr)
    );

    VLOG(4) << "IndexGroup(\"" << name << "\")" << "\n"
      << "\t" << "flag(" << group.get(&IndexGroup::flag) << ")" << "\n"
      << "\t" << "left_idx(" << group.get(&IndexGroup::left_index) << ")" << "\n"
      << "\t" << "right_idx(" << group.get(&IndexGroup::right_index) << ")" << "\n"
      << "\t" << "entry_id(" << group.get(&IndexGroup::entry_id) << ")" << "\n"
      << "\t" << "name_ptr(" << group.get(&IndexGroup::name_ptr) << ")" << "\n"
      << "\t" << "data_ptr(" << group.get(&IndexGroup::data_ptr) << ")";

    int32_t data_ptr = group.get(&IndexGroup::data_ptr);
    size_t data_offset = offset + data_ptr;

    // If it's within the bounds specified by the `root` section,
    // then this data entry must be another group. Treat it as one.
    if (is_group(data_offset)) {
      children.emplace_back(std::make_unique<folder>(
        name,
        consume_internal(data_offset)
      ));
    } else {
      // Read the sub-file size from the reader.
      uint32_t sub_file_size = _reader.read<uint32_t>(
        offset + group.get(&IndexGroup::data_ptr) + sizeof(char[4])
      );

      children.emplace_back(std::make_unique<file>(
        name,
        offset + group.get(&IndexGroup::data_ptr),
        sub_file_size
      ));
    } 
  }

  return children;
}