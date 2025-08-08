#include "repr.hh"

#include <sstream>

#include "lib/bytes/reader.hh"
#include "lib/bytes/typed_reader.hh"

#include "nodes/file.hh"
#include "nodes/folder.hh"

/**
 * The implementation of `repr`, taking a `reader` and the `curr` node.
 * Called by overloads of `repr` based on what parameters they have.
 */
static std::string _repr(
  std::optional<std::reference_wrapper<const typed_reader>> reader,
  const node& curr
);

/**
 * Walks the filesystem (DFS) to produce a tree, based on the current `depth`. `_repr` calls this with the appropriate values. 
 * Writes to the `stream` to produce output.
 * 
 * @param stream The stream to write results to.
 * @param reader The (optional) reader used to read file magics.
 * @param curr The current node.
 * @param depth The current depth.
 */
static void _repr_recurse(
  std::ostringstream& stream,
  std::optional<std::reference_wrapper<const typed_reader>> reader,
  const node& curr,
  int depth
);

/**
 * Produces the representation of the filesystem at `curr` as a tree.
 * Uses `reader` to read file magics.
 */
const std::string repr(const node& root, const typed_reader& reader) {
  return _repr(
    std::cref(reader),
    root
  );
}

/**
 * Produces the representation of the filesystem at `curr` as a tree.
 */
const std::string repr(const node& root) {
  return _repr(
    std::nullopt,
    root
  );
}

static std::string _repr(
  std::optional<std::reference_wrapper<const typed_reader>> reader,
  const node& curr
) {
  std::ostringstream stream;

  _repr_recurse(
    stream,
    reader,
    curr,
    0
  );

  return stream.str();
}

static void _repr_recurse(
  std::ostringstream& stream,
  std::optional<std::reference_wrapper<const typed_reader>> reader,
  const node& curr,
  int depth
) {
  std::string repeated(depth * 2, ' ');

  // If we have a reader, and the node is a `data`, 
  // try and output the `magic` for the file.
  //
  // If not, simply output the name.
  if (reader) {
    try {
      auto it = dynamic_cast<const file&>(curr);
      basic_array<char> magic = (*reader)
        .get()
        .read<char[]>(it.offset());

      stream << repeated << curr.name() << " (" << magic.at(0) << magic.at(1) << magic.at(2) << magic.at(3) << ")" << '\n'; 
    } catch (std::bad_cast& e) {
      stream << repeated << curr.name() << '\n';
    }
  } else {
    stream << repeated << curr.name() << '\n';
  }

  // If it's a `folder`, print its children.
  try {
    auto it = dynamic_cast<const folder&>(curr);
    
    for(const auto& child : it.children()) {
      _repr_recurse(
        stream,
        reader,
        child,
        depth + 1
      );
    }
  } catch (std::bad_cast& e) {};
}