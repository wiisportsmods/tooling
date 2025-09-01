cc_library(
  name = "stdlib",
  srcs = [],
  hdrs = [],
  visibility = ["//visibility:private"]
)

cc_library(
  name = "index_group",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.hh"]),
  includes = ["."],
  copts = ["-std=c++20"],
  strip_include_prefix = "/src",
  visibility = ["//visibility:public"],
  deps = [
    "@abseil-cpp//absl/log",
    "@abseil-cpp//absl/log:check",
    "//src/lib/bytes",
    "//src/lib/formats/brres",
    "//src/lib/fs",
    ":stdlib",
  ],
)