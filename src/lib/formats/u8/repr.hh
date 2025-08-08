#ifndef _LIB_FORMATS_U8_REPR_HH
#define _LIB_FORMATS_U8_REPR_HH

#include <string>

#include "lib/bytes/typed_reader.hh"
#include "nodes/node.hh"

/**
 * Prints a filesystem from the provided `root` `node`, using the `reader`
 * to read the magic values of the `data` nodes.
 * 
 * @param root The node to read from.
 * @param reader The reader to use to read magic values of the `data` nodes.
 * @returns A string representation of the filesystem, as a tree, including magic values for `data` nodes.
 */
const std::string repr(const node& root, const typed_reader& reader);

/**
 * Prints a filesystem from the provided `root` `node`.
 * 
 * @param root The node to begin from.
 * @returns A string representation of the filesystem, as a tree.
 */
const std::string repr(const node& root);

#endif /* _LIB_FORMATS_U8_REPR_HH */