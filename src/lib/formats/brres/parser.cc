#include "parser.hh"
#include "types.hh"

#include "nodes/node.hh"
#include "nodes/root.hh"
#include "nodes/folder.hh"
#include "nodes/data.hh"

#include "absl/log/log.h"

#include <iostream>

root parser::consume(const size_t base_offset) {
  return root(consume_internal<root>(base_offset));
}

template <typename TParent>
std::vector<std::reference_wrapper<node>> parser::consume_internal(size_t offset) {
  struct_reader<IndexGroupHeader> header = _reader.read<IndexGroupHeader>(
    offset
  );

  string_table table(_reader, offset);
  uint32_t count = header.get(&IndexGroupHeader::members);
  uint32_t byte_size = header.get(&IndexGroupHeader::byte_length);

  DLOG(INFO) << "Consuming group" << "\n"
    << "byte_size(" << byte_size << ")" << "\n"
    << "count(" << count << ")" << "\n"
    << "offset(" << offset << ")";

  struct_array<IndexGroup> groups = _reader.read<IndexGroup[]>(
    offset + sizeof(IndexGroupHeader)
  );

  std::vector<std::reference_wrapper<node>> children;
  children.reserve(count);

  for(uint32_t i = 1; i < count + 1; i++) {
    struct_reader<IndexGroup> group = groups.at(i);

    std::string name = table.copy(
      group.get(&IndexGroup::name_ptr)
    );

    DLOG(INFO) << "IndexGroup(\"" << name << "\")" << "\n"
      << "\t" << "left_idx(" << group.get(&IndexGroup::left_index) << ")" << "\n"
      << "\t" << "right_idx(" << group.get(&IndexGroup::right_index) << ")" << "\n"
      << "\t" << "entry_id(" << group.get(&IndexGroup::entry_id) << ")" << "\n"
      << "\t" << "name_ptr(" << group.get(&IndexGroup::name_ptr) << ")" << "\n"
      << "\t" << "data_ptr(" << group.get(&IndexGroup::data_ptr) << ")";

    if constexpr (std::is_same_v<TParent, root>) {
      _folders.emplace_back(
        name,
        consume_internal<folder>(offset + group.get(&IndexGroup::data_ptr))
      );

      children.emplace_back(std::ref(_folders.back()));
    }
    
    if constexpr (std::is_same_v<TParent, folder>) {
      _files.emplace_back(
        name,
        offset + group.get(&IndexGroup::data_ptr)
      );

      children.emplace_back(std::ref(_files.back()));
    }
  }

  return children;
}

const std::list<data>& parser::files() const {
  return _files;
}

const std::list<folder>& parser::folders() const {
  return _folders;
}