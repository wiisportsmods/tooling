#include <stack>

#include "absl/log/log.h"
#include "absl/log/check.h"
#include "absl/strings/str_format.h"


#include "lib/fs/nodes/node.hh"
#include "lib/fs/nodes/file.hh"
#include "lib/fs/nodes/folder.hh"
#include "lib/fs/repr.hh"

#include "parser.hh"
#include "parser_state.hh"

#include "types.hh"
#include "string_table.hh"

#include <algorithm>
#include <ranges>
#include <memory>

const uint16_t file_type = 0x0000;
const uint16_t folder_type = 0x0100;

std::string repr(
  const std::unordered_map<size_t, size_t>& m
) {
  std::ostringstream stream;

  for(auto it = m.cbegin(); it != m.cend(); ++it) {
    stream << it->first << " -> " << it->second;

    if (std::next(it) != m.cend()) {
      stream << '\n';
    }
  }

  return stream.str();
}

std::unique_ptr<node> parser::consume() {
  struct_reader<Header> header = _reader.read<Header>(0);

  uint32_t magic = header.get(&Header::magic);
  CHECK(magic == 0x55AA382D) << "Invalid magic: " << absl::StrFormat("0x%8x", magic);
  
  struct_array<Node> nodes = _reader.read<Node[]>(sizeof(Header));
  uint32_t node_count = nodes.at(0).get(&Node::size) - 1;
  
  uint32_t string_table_offset = header.get(&Header::root_offset) + ((node_count + 1) * sizeof(Node));
  string_table table(_reader, string_table_offset);

  detail::__parser_state state;

  for(size_t i = 0; i < node_count; i++) {
    struct_reader<Node> node = nodes.at(i);

    int16_t members = node.get(&Node::type) == file_type
      ? 0
      : node.get(&Node::size);

    state.visit(i, members);
  }

  return build_from_reverse_relationships(
    nodes,
    table,
    state.reverse_relationships()
  );
}

std::unique_ptr<node> parser::build_from_reverse_relationships(
  struct_array<Node>& nodes,
  const string_table& table,
  const std::unordered_multimap<size_t, size_t>& reverse_relationships
) {
  return build_from_reverse_relationships(
    nodes,
    table,
    reverse_relationships,
    0
  );
}

std::unique_ptr<node> parser::build_from_reverse_relationships(
  struct_array<Node>& nodes,
  const string_table& table,
  const std::unordered_multimap<size_t, size_t>& reverse_relationships,
  size_t curr
) {
  struct_reader<Node> curr_node = nodes.at(curr);
  uint16_t type = curr_node.get(&Node::type);

  VLOG(2) << absl::StrFormat(
    "node=%.3d: type=%.4x: %s",
    curr,
    type,
    type == file_type ? "file" : "folder"
  );

  if (type == file_type) {
    return std::make_unique<file>(
      table.copy(curr_node.get(&Node::name_offset)),
      curr_node.get(&Node::data_offset),
      curr_node.get(&Node::size)
    );
  } else {
    std::vector<std::unique_ptr<node>> children;    
    children.reserve(curr_node.get(&Node::size));

    auto range = reverse_relationships.equal_range(curr);

    for(auto it = range.first; it != range.second; ++it) {
      VLOG(4) << absl::StrFormat("Exporing (parent=%d, child=%d)", it->first, it->second);

      children.emplace_back(
        build_from_reverse_relationships(
          nodes,
          table,
          reverse_relationships,
          it->second
        )
      );

      VLOG(4) << absl::StrFormat("Expored %d", it->first);
    }

    std::string name = curr == 0
      ? "(root)" 
      : table.copy(curr_node.get(&Node::name_offset));

    return std::make_unique<folder>(
      name,
      std::move(children)
    );
  }
}
