#include "dump.hh"

#include <span>
#include <fstream>

#include "absl/log/check.h"

#include "nodes/file.hh"
#include "nodes/folder.hh"

/**
 * Implements the dumping logic as a recursive function (DFS).
 * 
 * @param reader The reader used to produce this filesystem, which is used to copy
 *    contents from.
 * @param base The base path, used for logging
 * @param path The current path the search is visiting.
 * @param curr The current node the search is visiting.
 * @returns void
 */
static void _dump_recurse(
  const byte_reader& reader,
  const std::filesystem::path base,
  const std::filesystem::path path,
  const std::unique_ptr<node>& curr
);

// This is actually a wrapper, providing a cleaner implementation over 
// the recursion (since `base` and `path` are needed for logging)
void dump(
  const byte_reader& reader,
  const std::filesystem::path base,
  const std::unique_ptr<node>& root
) {
  _dump_recurse(
    reader,
    base,
    base,
    root
  );
}

static void _dump_recurse(
  const byte_reader& reader,
  const std::filesystem::path base,
  const std::filesystem::path path,
  const std::unique_ptr<node>& curr
) {
  std::filesystem::path curr_dir(path / curr->name());

  {
    const file* f = dynamic_cast<file*>(curr.get());
    
    if (f != nullptr) {
      std::ofstream output(curr_dir);
      std::span<char> span = reader.span<char>(f->offset(), f->size());
      output.write(span.data(), span.size());

      return;
    }
  }

  {
    const folder* f = dynamic_cast<folder*>(curr.get());

    if (f != nullptr) {
      if (!std::filesystem::is_directory(curr_dir)) {
        std::filesystem::create_directories(curr_dir);
      }

      for(const auto& child : f->children()) {
        _dump_recurse(
          reader,
          base,
          curr_dir,
          child
        );
      }

      return;
    }
  }

  CHECK(false) << "unreachable: node is not a file or folder";
}