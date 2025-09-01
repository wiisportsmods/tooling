cc_library(
  name = "stdlib",
  srcs = [],
  hdrs = [],
  visibility = ["//visibility:private"]
)

cc_library(
  name = "lib",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.hh"]),
  includes = ["."],
  copts = ["-std=c++20"],
  strip_include_prefix = "/src",
  visibility = ["//visibility:public"],
  deps = [
    ":stdlib",
    "@abseil-cpp//absl/strings:str_format",
  ],
)