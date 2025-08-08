#include <fstream>
#include <filesystem>
#include <stdlib.h>

#include "absl/log/log.h"
#include "absl/log/check.h"
#include "absl/strings/str_format.h"

#include "lib/bytes/reader.hh"
#include "lib/bytes/typed_reader.hh"

#include "lib/formats/u8/parser.hh"
#include "lib/formats/u8/nodes/folder.hh"
#include "lib/formats/u8/repr.hh"

#include "lib/compression/yaz0/parser.hh"

#include "types.hh"

#include <iostream>

void write(
  const byte_reader& reader,
  const std::filesystem::path base,
  const node& curr,
  const std::filesystem::path path
);

/**
 * Format a number of `bytes` as a string w/ units.
 */
std::string format_size(size_t byte_length) {
  static const std::array<const std::string, 4> size_lookup = {"b", "kb", "mb", "gb"};

  size_t index = 0;
  while(byte_length > 1023) {
    byte_length /= 1024;
    index = std::min(index + 1, (size_t)3);
  }

  return absl::StrFormat("%d%s", byte_length, size_lookup[index]);
}

int main(
  int argc,
  char** argv
) {
  CHECK(argc == 3) << "Invalid usage";

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
  
  LOG(INFO) << "File size: " << format_size(len);

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

  parser u8_parser(u8_reader);
  folder& root = u8_parser.consume();

  LOG(INFO) << repr(root);

  std::filesystem::path base_dir(destination);

  LOG(INFO) << "Cleaning " << destination;
  std::filesystem::remove_all(destination);
  std::filesystem::create_directory(base_dir);

  LOG(INFO) << "Writing " << destination;
  write(u8_reader, destination, root, destination);
}

void write(
  const byte_reader& reader,
  const std::filesystem::path base,
  const node& curr,
  const std::filesystem::path path
) {
  try {
    const file& f = dynamic_cast<const file&>(curr);

    const std::filesystem::path dir(path / f.name());
    LOG(INFO) << "Creating file " << dir;

    std::ofstream file(dir);
    std::span<char> span = reader.span<char>(f.offset(), f.size());

    file.write(span.data(), span.size());
  } catch (std::bad_cast& e) {}

  try {
    const folder& f = dynamic_cast<const folder&>(curr);

    const std::filesystem::path dir(path / f.name());

    LOG(INFO) << "Creating directory " << dir;

    if (!std::filesystem::is_directory(dir)) {
      std::filesystem::create_directories(dir);
    }

    for(const auto& child : f.children()) {
      write(reader, base, child, dir);
    }
  } catch (std::bad_cast& e) {}
}