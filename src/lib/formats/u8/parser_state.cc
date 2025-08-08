#include "parser_state.hh"

#include "absl/strings/str_format.h"

namespace detail {
  size_t __parser_node_state::index() {
    return _idx;
  }

  void __parser_node_state::decrement() {
    if (finished()) {
      return;
    }

    _count--;
  }

  bool __parser_node_state::finished() {
    return _count == 0;
  }

  void __parser_state::visit(
    size_t node,
    size_t members
  ) {
    next();

    if (!finished()) {
      size_t parent_idx = peek() 
        .value()
        .get()
        .index();
      
      _tree.insert({
        node,
        parent_idx
      });
    }

    _stack[++_ptr] = __parser_node_state(node, members);
  }

  void __parser_state::next() {
    if (finished()) {
      return;
    }

    __parser_node_state& head = _stack[_ptr];

    head.decrement();
    if (head.finished()) {
      _ptr--;
    }
  }

  bool __parser_state::finished() {
    return _ptr == -1;
  }

  const std::unordered_map<size_t, size_t>& __parser_state::relationships() {
    return _tree;
  }

  const std::unordered_multimap<size_t, size_t> __parser_state::reverse_relationships() {
    std::unordered_multimap<size_t, size_t> multi;
    auto r = relationships();

    for(auto it = r.cbegin(); it != r.cend(); ++it) {      
      multi.insert({ it->second, it->first });
    }

    return multi;
  }

  std::optional<std::reference_wrapper<__parser_node_state>> __parser_state::peek() {
    if (finished()) {
      return std::nullopt;
    }

    return std::ref(_stack[_ptr]);
  }
}