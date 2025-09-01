#include <fstream>
#include <filesystem>
#include <stdlib.h>

#include "absl/log/log.h"
#include "absl/log/check.h"
#include "absl/strings/str_format.h"

#include "lib/bytes/reader.hh"
#include "lib/bytes/typed_reader.hh"
#include "lib/bytes/array/basic.hh"
#include "lib/bytes/array/basic.hh"

#include "lib/formats/brres/index_group/parser.hh"
#include "lib/formats/brres/types.hh"

#include "lib/fs/repr.hh"
#include "lib/fs/dump.hh"

#include "types.hh"

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

  std::filesystem::path input(argv[1]);
  std::filesystem::path output(argv[2]);

  std::string source(input);

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
  byte_reader binary(std::move(buffer), len);

  typed_reader reader(binary);
  {
    basic_array<char> magic_reader = reader.read<char[]>(0);

    char magic[4] = {
      magic_reader.at(0),
      magic_reader.at(1),
      magic_reader.at(2),
      magic_reader.at(3)
    };

    // `brres` files always start with `bres` in the first four bytes.
    CHECK(memcmp(magic, "bres", 4) == 0)  << "Invalid file magic";
  }

  // Read information about the file (bom, sections, offset to the root, etc).
  struct_reader<Header> header = reader.read<Header>(0);

  uint16_t bom = header.get(&Header::bom);
  if (bom != 0xFEFF) {
    binary.swap_endianness();
    LOG(INFO) << "Endianness mismatch - reader will automatically swap the byte order";
  }

  const uint16_t section_count = header.get(&Header::num_sections);
  LOG(INFO) << "bom=" << absl::StrFormat("0x%04X", bom) << " section_count=" << section_count << " len=" << len;

  const uint16_t root_section_offset = header.get(&Header::root_offset);
  
  struct_reader<RootSectionHeader> root_header = reader.read<RootSectionHeader>(root_section_offset);
  {
    basic_array<char> magic_reader = root_header.get(&RootSectionHeader::magic);

    char magic[4] = {
      magic_reader.at(0),
      magic_reader.at(1),
      magic_reader.at(2),
      magic_reader.at(3)
    };

    // `root` sections always have a magic of `root`.
    CHECK(memcmp(magic, "root", 4) == 0)  << "Invalid section magic: "
      << magic[0]
      << magic[1]
      << magic[2]
      << magic[3];
  }
  
  // Create a BRRES `Index Group` parser, and parse out the values.
  parser index_group_parser(
    reader,
    root_section_offset + sizeof(RootSectionHeader),
    (size_t)root_header.get<uint32_t>(&RootSectionHeader::byte_length)
  );
  
  // Show the tree of files.
  // LOG(INFO) << repr(brres_root, reader);

  LOG(INFO) << "Cleaning " << output;
  std::filesystem::remove_all(output);
  std::filesystem::create_directory(output);

  LOG(INFO) << "Writing " << output;

  dump(
    binary,
    output,
    index_group_parser.consume(
      input.filename().string() + ".out"
    )
  );
}