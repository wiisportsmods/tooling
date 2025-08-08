#ifndef _LIB_FORMATS_U8_PARSER_STATE_HH
#define _LIB_FORMATS_U8_PARSER_STATE_HH

#include <stdlib.h>

#include <array>
#include <optional>
#include <unordered_map>
#include <map>

namespace detail {
  const size_t MAX_DEPTH = 32;
  
  class __parser_node_state {
    size_t _idx;
    size_t _count;

  public:
    __parser_node_state() : __parser_node_state(0, 0) {}
    __parser_node_state(
      size_t idx,
      size_t count
    ) : _idx(idx), _count(count) {}

    void decrement();
    bool finished();

    size_t index();
  };

  /**
   * Used to track state within the `consume()` method of
   * `parser`.
   * 
   * @internal
   */
  class __parser_state {
    // const size_t _current_node;
    
    std::array<__parser_node_state, MAX_DEPTH> _stack;
    
    /**
     * A map of child -> parent. You can traverse up this map to find
     * the relationship between a child to its parent.
     * 
     * If an index isn't present in this map, it must be the root.
     */
    std::unordered_map<size_t, size_t> _tree;

    int16_t _ptr;
  
  public:
    __parser_state() : _ptr(-1) {}

    void visit(size_t node, size_t members);
    void next();
    bool finished();

    std::optional<std::reference_wrapper<__parser_node_state>> peek();
    const std::unordered_map<size_t, size_t>& relationships();
    const std::unordered_multimap<size_t, size_t> reverse_relationships();
  };
}

#endif /* _LIB_FORMATS_U8_PARSER_STATE_HH */