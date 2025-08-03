#ifndef _LIB_FORMATS_BRRES_HH
#define _LIB_FORMATS_BRRES_HH

#include <optional>
#include <vector>
#include <span>
#include <expected>


class fs_entry {
  std::string _name;
  std::vector<fs_entry> _children;
  const bool _is_root;

  // std::optional<std::span<uint8_t>> _data;

public:
  fs_entry(const std::string name)
    : fs_entry(name, false) {}
  
  fs_entry(const std::string name, const bool is_root)
    : _name(name), _is_root(is_root) {}

  const std::string& name();
  std::vector<fs_entry>& children();
  const bool root();
  // const std::optional<std::span<uint8_t>> data();
};

#endif /* _LIB_FORMATS_BRRES_HH */