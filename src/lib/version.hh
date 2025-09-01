#ifndef _LIB_VERSION_HH
#define _LIB_VERSION_HH

#include <inttypes.h>
#include <string>

#include "absl/strings/str_format.h"

namespace version {
  static const uint16_t major = 0;
  static const uint16_t minor = 0;
  static const uint16_t revision = 0;

  #ifndef RELEASE_STREAM
    static const char* release_stream = "dev";
  #else
    static const char* release_stream = "RELEASE_STREAM";
  #endif

  inline std::string str() {
    return absl::StrFormat(
      "%d.%d-%d#%s",
      major,
      minor,
      revision,
      release_stream
    );
  }
}

#endif /* _LIB_VERSION_HH */