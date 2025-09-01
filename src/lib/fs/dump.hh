#ifndef _LIB_FS_DUMP_HH
#define _LIB_FS_DUMP_HH

#include <filesystem>

#include "lib/bytes/reader.hh"

#include "nodes/node.hh"

/**
 * Dump the virtual filesystem to the system filesystem, starting
 * with `root` as the root node, using a DFS.
 * 
 * @param reader The reader used to parse the filesystem originally,
 *    since offsets are relative to this reader.
 * @param base The base path to dump to.
 * @param root The node to start the search at.
 */
void dump(
  const byte_reader& reader,
  const std::filesystem::path base,
  const std::unique_ptr<node>& root
);

#endif /* _LIB_FS_DUMP_HH */