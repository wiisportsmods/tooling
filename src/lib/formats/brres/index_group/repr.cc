#include "repr.hh"

#include <sstream>

#include "lib/bytes/reader.hh"
#include "lib/bytes/typed_reader.hh"

#include "nodes/data.hh"

static std::string _repr(
  std::optional<std::reference_wrapper<const typed_reader>> reader,
  const node& curr
);

static void _repr_recurse(
  std::ostringstream& stream,
  std::optional<std::reference_wrapper<const typed_reader>> reader,
  const node& curr,
  int depth
);

const std::string repr(const node& root, const typed_reader& reader) {
  return _repr(
    std::cref(reader),
    root
  );
}

const std::string repr(const node& root) {
  return _repr(
    std::nullopt,
    root
  );
}

static std::string _repr(
  std::optional<std::reference_wrapper<const typed_reader>> reader,
  const node& curr
) {
  std::ostringstream stream;

  _repr_recurse(
    stream,
    reader,
    curr,
    0
  );

  return stream.str();
}

static void _repr_recurse(
  std::ostringstream& stream,
  std::optional<std::reference_wrapper<const typed_reader>> reader,
  const node& curr,
  int depth
) {
  std::string repeated(depth * 2, ' ');

  if (reader) {
    try {
      auto it = dynamic_cast<const data&>(curr);
      basic_array<char> magic = (*reader)
        .get()
        .read<char[]>(it.offset());

      stream << repeated << curr.name() << " (" << magic.at(0) << magic.at(1) << magic.at(2) << magic.at(3) << ")" << '\n'; 
    } catch (std::bad_cast& e) {
      stream << repeated << curr.name() << '\n';
    }
  } else {
    stream << repeated << curr.name() << '\n';
  }

  if (!curr.children()) {
    return;
  }

  for(const auto& child : curr.children().value().get()) {
    _repr_recurse(
      stream,
      reader,
      child,
      depth + 1
    );
  }
}