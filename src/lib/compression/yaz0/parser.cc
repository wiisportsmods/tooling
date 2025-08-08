#include "parser.hh"

#include "absl/log/check.h"
#include "absl/log/log.h"

#include "absl/strings/str_format.h"

#include "types.hh"

std::string bits_of(uint8_t byte) {
  return std::bitset<8>(byte).to_string();
}

DecompressResult decompress(const typed_reader& reader) {
  struct_reader<Header> header = reader.read<Header>(0);
  basic_array<char> magic_reader = header.get(&Header::magic);

  char magic[4] = {
    magic_reader.at(0),
    magic_reader.at(1),
    magic_reader.at(2),
    magic_reader.at(3)
  };

  CHECK(memcmp(magic, "Yaz0", 4) == 0) << "Invalid magic: "
    << magic[0] << magic[1] << magic[2] << magic[3];

  size_t src_len = reader.length();
  size_t dst_len = header.get(&Header::uncompressed_size);

  size_t src_idx = sizeof(Header);
  size_t dst_idx = 0;

  size_t group_size = 0;
  uint8_t group_head = 0;

  std::unique_ptr<char[]> output = std::make_unique<char[]>(dst_len);

  while (src_idx < src_len && dst_idx < dst_len)
  {
    if (group_size == 0)
    {
      group_head = reader.read<uint8_t>(src_idx++);
      group_size = 8;
    }

    if (group_head & 0x80) {
      char byte = reader.read<uint8_t>(src_idx);
      output[dst_idx] = byte;

      src_idx++;
      dst_idx++;
    }  else {
      const uint8_t b1 = reader.read<uint8_t>(src_idx);
      const uint8_t b2 = reader.read<uint8_t>(src_idx + 1);
      src_idx += 2;

      uint32_t distance = ((b1 & 0x0F) << 8) | b2;
      uint32_t copy_src_idx = dst_idx - (distance + 1);
      uint32_t num_bytes = b1 >> 4;

      if (num_bytes == 0) {
        num_bytes = reader.read<uint8_t>(src_idx) + 0x12;
        src_idx++;
      } else {
        num_bytes += 2;
      }

      CHECK(num_bytes >= 3 && num_bytes <= 0x111) << "num_bytes out of range " << num_bytes;

      for(uint32_t i = 0; i < num_bytes; i++) {
        output[dst_idx] = output[copy_src_idx];

        dst_idx++;
        copy_src_idx++;
      }
    }

    group_size--;
    group_head <<= 1;
  }

  CHECK(src_idx <= src_len);
  CHECK(dst_idx <= dst_len);

  DecompressResult result;
  
  result.buffer = std::move(output);
  result.length = dst_len;

  return result;
}