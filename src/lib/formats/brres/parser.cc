#include "parser.hh"
#include "types.hh"

#include "absl/log/log.h"

#include <iostream>

fs_entry& parser::consume(const size_t base_offset) {
  // The base entry, used to represent the root of this.

  LOG(INFO) << "Consuming Index Groups";

  fs_entry root("__root__", true);
  _entries.emplace_back(root);
  fs_entry& base = _entries.back();

  LOG(INFO) << "Consuming Children (call)";

  consume_children(
    base,
    base_offset
  );

  return _entries[0];
}

void parser::consume_children(
  fs_entry& parent,
  size_t offset
) {
  struct_reader<IndexGroupHeader> header = _reader.read<IndexGroupHeader>(
    offset
  );

  string_table table(_reader, offset);
  uint32_t count = header.get(&IndexGroupHeader::members);
  uint32_t byte_size = header.get(&IndexGroupHeader::byte_length);

  LOG(INFO) << "Consuming Children of " << parent.name() << "\n"
    << "byte_size(" << byte_size << ")" << "\n"
    << "count(" << count << ")" << "\n"
    << "offset(" << offset << ")";

  struct_array<IndexGroup> groups = _reader.read<IndexGroup[]>(
    offset + sizeof(IndexGroupHeader)
  );

  for(uint32_t i = 1; i < count + 1; i++) {
    struct_reader<IndexGroup> group = groups.at(i);


    std::string name = table.copy(
      group.get(&IndexGroup::name_ptr)
    );

    LOG(INFO) << "IndexGroup(\"" << name << "\")" << "\n"
      << "\t" << "left_idx(" << group.get(&IndexGroup::left_index) << ")" << "\n"
      << "\t" << "right_idx(" << group.get(&IndexGroup::right_index) << ")" << "\n"
      << "\t" << "entry_id(" << group.get(&IndexGroup::entry_id) << ")" << "\n"
      << "\t" << "name_ptr(" << group.get(&IndexGroup::name_ptr) << ")" << "\n"
      << "\t" << "data_ptr(" << group.get(&IndexGroup::data_ptr) << ")";

    parent.children().emplace_back(name, false);
    
    if (parent.root()) {
      consume_children(
        parent.children().back(),
        offset + group.get(&IndexGroup::data_ptr)
      );
    }
  }
}