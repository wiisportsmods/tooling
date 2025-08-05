cc_library(
  name = "stdlib",
  srcs = [],
  hdrs = [],
  visibility = ["//visibility:private"]
)

cc_library(
  name = "brres",
  srcs = glob(["*.cc", "nodes/*.cc"]),
  hdrs = glob(["*.hh", "nodes/*.hh"]),
  includes = ["."],
  copts = ["-std=c++20"],
  strip_include_prefix = "/src",
  visibility = ["//visibility:public"],
  deps = [
    "@com_google_absl//absl/log",
    "@com_google_absl//absl/log:check",
    "//src/lib/bytes",
    ":stdlib",
  ],
)