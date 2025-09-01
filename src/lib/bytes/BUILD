cc_library(
  name = "stdlib",
  srcs = [],
  hdrs = [],
  visibility = ["//visibility:private"]
)

cc_library(
  name = "bytes",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.hh", "array/*.hh"]),
  includes = ["."],
  copts = ["-std=c++20"],
  strip_include_prefix = "/src",
  visibility = ["//visibility:public"],
  deps = [
    "@abseil-cpp//absl/log",
    "@abseil-cpp//absl/log:check",
    ":stdlib",
  ],
)