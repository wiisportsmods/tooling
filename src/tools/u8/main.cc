#include <fstream>
#include <filesystem>
#include <iostream>

#include <stdlib.h>

#include "absl/flags/parse.h"
#include "absl/flags/flag.h"

#include "absl/log/log.h"
#include "absl/log/flags.h"
#include "absl/log/check.h"
#include "absl/strings/str_format.h"

#include "lib/bytes/reader.hh"
#include "lib/bytes/typed_reader.hh"

#include "lib/compression/yaz0/parser.hh"

#include "lib/formats/u8/parser.hh"

#include "lib/fs/dump.hh"
#include "lib/fs/repr.hh"
#include "lib/fs/nodes/node.hh"

#include "lib/size.hh"


#include "types.hh"

int main(
  int argc,
  char** argv
) {
  absl::ParseCommandLine(argc, argv);

  CHECK(argc >= 3) << "Invalid usage";

  std::string source(argv[1]);
  std::string destination(argv[2]);

  LOG(INFO) << "Opening " << source << 
    "(cwd=" << std::filesystem::current_path() << ")";
  
  std::ifstream file(source);

  CHECK(file.is_open()) << "Failed to open file (errno=" << errno << ", msg='" << strerror(errno) << "')"; 

  file.seekg(0, std::ios::end);
  size_t len = file.tellg();
  file.seekg(0, std::ios::beg);

  CHECK(len != 0) << "Empty file";
  
  LOG(INFO) << "File size: " << format_byte_size(len);

  std::unique_ptr<char[]> buffer = std::make_unique<char[]>(len + 1);
  CHECK(buffer != nullptr) << "Failed to allocate buffer";

  buffer[len] = '\0';

  file.read(buffer.get(), len);
  // Create readers over the binary.
  byte_reader yaz_compressed_bin(std::move(buffer), len);
  typed_reader yaz_reader(yaz_compressed_bin);

  yaz_compressed_bin.swap_endianness();

  LOG(INFO) << "Decompressing";
  DecompressResult decompressed = decompress(yaz_reader);

  byte_reader u8_reader(std::move(decompressed.buffer), decompressed.length);
  u8_reader.swap_endianness();

  LOG(INFO) << "Parsing";

  typed_reader reader(u8_reader);
  parser u8_parser(reader);

  std::unique_ptr<node> root = u8_parser.consume();

  LOG(INFO) << repr(root);

  std::filesystem::path base_dir(destination);

  LOG(INFO) << "Cleaning " << destination;
  std::filesystem::remove_all(destination);
  std::filesystem::create_directory(base_dir);

  LOG(INFO) << "Writing " << destination;

  dump(
    u8_reader,
    destination,
    root
  );
}