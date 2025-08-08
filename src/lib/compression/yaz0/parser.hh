#ifndef _LIB_COMPRESSION_YAZ0_PARSER_HH
#define _LIB_COMPRESSION_YAZ0_PARSER_HH

#include <memory>

#include "lib/bytes/typed_reader.hh"

struct DecompressResult {


  std::unique_ptr<char[]> buffer;
  uint32_t length;
};

DecompressResult decompress(const typed_reader& reader);

#endif /* _LIB_COMPRESSION_YAZ0_PARSER_HH */