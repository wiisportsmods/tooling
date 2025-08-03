#include <fstream>

#include "absl/log/log.h"
#include "absl/log/check.h"
#include "absl/strings/str_format.h"

#include "lib/bytes/reader.hh"
#include "lib/bytes/typed_reader.hh"
#include "lib/bytes/array/basic.hh"
#include "lib/bytes/array/struct.hh"

#include "lib/formats/brres/parser.hh"
#include "lib/formats/brres/string_table.hh"
#include "lib/formats/brres/types.hh"

#include "types.hh"

void print_tree(
  fs_entry& root,
  int depth
);

int main(
  int argc,
  char** argv
) {
  CHECK(argc == 2) << "Invalid usage";

  std::string source(argv[1]);

  LOG(INFO) << "Opening " << source;
  
  std::ifstream file(source);

  CHECK(file.is_open()) << "Failed to open file (errno=" << errno << ", msg='" << strerror(errno) << "')"; 

  file.seekg(0, std::ios::end);
  size_t len = file.tellg();
  file.seekg(0, std::ios::beg);

  CHECK(len != 0) << "Empty file";

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
    bom = header.get(&Header::bom);
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

  struct_reader<IndexGroupHeader> index_group_header = reader.read<IndexGroupHeader>(
    root_section_offset + sizeof(RootSectionHeader)
  );

  uint32_t members = index_group_header.get(&IndexGroupHeader::members);

  LOG(INFO) << "root members: " << members;
  LOG(INFO) << "root size: " << index_group_header.get(&IndexGroupHeader::byte_length);

  struct_array<IndexGroup> group_array = reader.read<IndexGroup[]>(
    root_section_offset + sizeof(RootSectionHeader) + sizeof(IndexGroupHeader)
  );

  size_t base_offset = root_section_offset + sizeof(RootSectionHeader);
  string_table table(reader, base_offset);  

  parser index_group_parser(reader);
  fs_entry& root = index_group_parser.consume(base_offset);
  print_tree(root, 0);

  file.close();
  delete[] buffer;
}

void print_tree(
  fs_entry& root,
  int depth
) {
  for(auto& child : root.children()) {
    std::string repeated(depth, '  ');
    LOG(INFO) << repeated << child.name();
    print_tree(child, depth + 1);
  }
}