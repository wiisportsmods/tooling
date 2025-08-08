cc_library(
  name = "stdlib",
  srcs = [],
  hdrs = [],
  visibility = ["//visibility:private"]
)

cc_library(
  name = "yaz0",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.hh"]),
  includes = ["."],
  copts = ["-std=c++20"],
  strip_include_prefix = "/src",
  visibility = ["//visibility:public"],
  deps = [
    "//src/lib/bytes",
    "@com_google_absl//absl/log:check",
    "@com_google_absl//absl/log",
    "@com_google_absl//absl/strings",
    ":stdlib",
  ],
)