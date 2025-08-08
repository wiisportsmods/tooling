#include <stack>

#include "absl/log/check.h"
#include "absl/strings/str_format.h"


#include "nodes/node.hh"

#include "parser.hh"
#include "parser_state.hh"

#include "types.hh"
#include "string_table.hh"

#include <algorithm>
#include <ranges>

std::string repr(
  const std::unordered_multimap<size_t, size_t>& map
) {
  std::ostringstream stream;

  stream << "{\n";

  for(auto it = map.begin(); it != map.end(); ++it) {
    stream << "\t" << it->first << " -> " << it->second << "\n";
  }

  stream << "}";

  return stream.str();
}

std::string as_relationship_graph(
  const std::unordered_map<size_t, size_t>& map
) {
  std::ostringstream stream;
  stream << "{\n";

  for(auto it = map.cbegin(); it != map.cend(); ++it) {
    std::deque<size_t> d;
    stream << "\t";

    size_t key = it->first;

    while (map.find(key) != map.end()) {
      d.push_back(key);
      key = map.at(key);
    }

    d.push_back(0);

    for(auto dit = d.crbegin(); dit != d.crend(); ++dit) {
      stream << *dit;

      if (std::next(dit) != d.crend()) {
        stream << " -> ";
      }
    }

    stream << "\n";
  }

  stream << "}";

  return stream.str();
}

folder& parser::consume() {
  struct_reader<Header> header = _reader.read<Header>(0);

  uint32_t magic = header.get(&Header::magic);
  CHECK(magic == 0x55AA382D) << "Invalid magic: " << absl::StrFormat("0x%8x", magic);
  
  struct_array<Node> nodes = _reader.read<Node[]>(sizeof(Header));
  struct_reader<Node> root_node = nodes.at(0);
  uint32_t node_count = root_node.get(&Node::size) - 1;

  uint32_t string_table_offset = header.get(&Header::root_offset) + ((node_count + 1) * sizeof(Node));

  string_table table(
    _reader,
    string_table_offset
  );

  detail::__parser_state state;

  std::vector<std::reference_wrapper<node>> all;
  all.reserve(node_count);

  for(size_t i = 0; i < node_count; i++) {
    struct_reader<Node> node = nodes.at(i);

    int16_t members = node.get(&Node::type) == file::TYPE
      ? 0
      : node.get(&Node::size);

    state.visit(i, members);
  }

  folder& root = build_from_reverse_relationships(
    nodes,
    table,
    state.reverse_relationships()
  );

  return root;
}

folder& parser::build_from_reverse_relationships(
  struct_array<Node>& nodes,
  const string_table& table,
  const std::unordered_multimap<size_t, size_t>& reverse_relationships
) {
  return dynamic_cast<folder&>(build_from_reverse_relationships(
    nodes,
    table,
    reverse_relationships,
    0
  ));
}

node& parser::build_from_reverse_relationships(
  struct_array<Node>& nodes,
  const string_table& table,
  const std::unordered_multimap<size_t, size_t>& reverse_relationships,
  size_t curr
) {
  struct_reader<Node> curr_node = nodes.at(curr);
  uint16_t type = curr_node.get(&Node::type);

  if (type == file::TYPE) {
    _files.emplace_back(
      table.copy(curr_node.get(&Node::name_offset)),
      curr_node.get(&Node::data_offset),
      curr_node.get(&Node::size)
    );

    return _files.back();

  } else {
    std::vector<std::reference_wrapper<node>> children;
    children.reserve(curr_node.get(&Node::size));

    auto range = reverse_relationships.equal_range(curr);

    for(auto it = range.first; it != range.second; ++it) {
      children.push_back(
        build_from_reverse_relationships(
          nodes,
          table,
          reverse_relationships,
          it->second
        )
      );
    }

    std::string name = curr == 0
      ? "(root)" 
      : table.copy(curr_node.get(&Node::name_offset));
    
    _folders.emplace_back(
      name,
      children
    );

    return _folders.back();
  }
}
