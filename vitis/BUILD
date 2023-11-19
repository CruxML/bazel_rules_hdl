exports_files(["vitis_generate.tcl.template"])

# Define versions of vitis_hls/vivado_hls as needed.
# These are copied from the installation.
cc_library(
    name = "v2021_2_cc",
    srcs = glob([
        "v2021_2/**/*.h",
    ]) + glob([
        "v2021_2/**/*.cpp",
    ]),
    strip_include_prefix = "external/rules_hdl/",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "v2020_1_cc",
    srcs = glob([
        "v2020_1/**/*.h",
    ]),
    strip_include_prefix = "external/rules_hdl/",
    visibility = ["//visibility:public"],
)

py_binary(
    name = "hls_generator",
    srcs = ["hls_generator.py"],
    visibility = ["//visibility:public"],
)
