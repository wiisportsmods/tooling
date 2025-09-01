cc_library(
  name = "stdlib",
  srcs = [],
  hdrs = [],
  visibility = ["//visibility:private"]
)

cc_library(
  name = "fs",
  srcs = glob(["*.cc", "nodes/*.cc"]),
  hdrs = glob(["*.hh", "nodes/*.hh"]),
  includes = ["."],
  copts = ["-std=c++20"],
  strip_include_prefix = "/src",
  visibility = ["//visibility:public"],
  deps = [
    "@abseil-cpp//absl/log:check",
    "//src/lib/bytes",
    ":stdlib",
  ],
)