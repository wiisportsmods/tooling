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

#include "lib/formats/brres/parser.hh"
#include "lib/formats/brres/nodes/node.hh"
#include "lib/formats/brres/types.hh"

#include "types.hh"

void print_tree(
  typed_reader& reader,
  const node& curr,
  int depth
);

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
  CHECK(argc == 2) << "Invalid usage";

  std::string source(argv[1]);

  LOG(INFO) << "Opening " << source << 
    "(cwd=" << std::filesystem::current_path() << ")";
  
  std::ifstream file(source);

  CHECK(file.is_open()) << "Failed to open file (errno=" << errno << ", msg='" << strerror(errno) << "')"; 

  file.seekg(0, std::ios::end);
  size_t len = file.tellg();
  file.seekg(0, std::ios::beg);

  CHECK(len != 0) << "Empty file";
  
  LOG(INFO) << "File size: " << format_size(len);

  char* buffer = new char[len+1];

  CHECK(buffer != nullptr) << "Failed to allocate buffer";
  buffer[len] = '\0';

  file.read(buffer, len);

  byte_reader binary(buffer, len);

  typed_reader reader(binary);
  {
    basic_array<char> magic_reader = reader.read<char[]>(0);

    char magic[4] = {
      magic_reader.at(0),
      magic_reader.at(1),
      magic_reader.at(2),
      magic_reader.at(3)
    };

    CHECK(memcmp(magic, "bres", 4) == 0)  << "Invalid file magic";
  }

  struct_reader<Header> header = reader.read<Header>(0);

  uint16_t bom = header.get(&Header::bom);
  if (bom != 0xFEFF) {
    binary.swap_endianness();
    LOG(INFO) << "Endianness mismatch - reader will automatically swap the byte order";
  }

  const uint16_t section_count = header.get(&Header::num_sections);
  LOG(INFO) << "bom=" << absl::StrFormat("0x%04X", bom) << " section_count=" << section_count << " len=" << len;

  const uint16_t root_section_offset = header.get(&Header::root_offset);
  
  {
    struct_reader<RootSectionHeader> root_header = reader.read<RootSectionHeader>(root_section_offset);
    basic_array<char> magic_reader = root_header.get(&RootSectionHeader::magic);

    char magic[4] = {
      magic_reader.at(0),
      magic_reader.at(1),
      magic_reader.at(2),
      magic_reader.at(3)
    };

    CHECK(memcmp(magic, "root", 4) == 0)  << "Invalid section magic: "
      << magic[0]
      << magic[1]
      << magic[2]
      << magic[3];
  }
  
  size_t base_offset = root_section_offset + sizeof(RootSectionHeader);
  parser index_group_parser(reader);

  root root = index_group_parser.consume(base_offset);
  print_tree(reader, root, 0);

  file.close();
  delete[] buffer;
}

void print_tree(
  typed_reader& reader,
  const node& curr,
  int depth
) {
  std::string repeated(depth * 2, ' ');

  try {
    auto it = dynamic_cast<const data&>(curr);
    basic_array<char> magic = reader.read<char[]>(it.offset());

    LOG(INFO) << repeated << curr.name () << " (" << magic.at(0) << magic.at(1) << magic.at(2) << magic.at(3) << ")"; 
  } catch (std::bad_cast& e) {
    LOG(INFO) << repeated << curr.name();
  }

  if (!curr.children()) {
    return;
  }

  for(const auto& child : curr.children().value().get()) {
    print_tree(reader, child, depth + 1);
  }
}