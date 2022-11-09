load("//vitis:defs.bzl", "vitis_generate")

# This is to test the path of generated headers.
genrule(
  name = "consts",
  cmd_bash = "touch $(OUTS)",
  outs = [
    "consts.h",
  ],
)

cc_library(
    name = "consts_cc",
    srcs = [
        "consts.h",
    ],
)

cc_library(
    name = "hls_adder",
    srcs = [
        "hls_adder.cc",
    ],
    hdrs = [
        "hls_adder.h",
    ],
    deps = [
        "consts_cc",
        "@rules_hdl//vitis:v2021_2_cc",
    ],
)

cc_test(
    name = "hls_adder_test",
    srcs = [
        "hls_adder_test.cc",
    ],
    deps = [
        ":hls_adder",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "hls_adder_vivado",
    srcs = [
        "hls_adder.cc",
    ],
    hdrs = [
        "hls_adder.h",
    ],
    defines = ["WITH_VIVADO_HLS=1"],
    deps = [
        "@rules_hdl//vitis:v2020_1_cc",
    ],
)

cc_test(
    name = "hls_adder_vivado_test",
    srcs = [
        "hls_adder_test.cc",
    ],
    deps = [
        ":hls_adder_vivado",
        "@com_google_googletest//:gtest_main",
    ],
)

vitis_generate(
    name = "adder",
    out = "adder.tar.gz",
    clock_period = "10.0",
    tags = ["manual"],
    top_func = "adder",
    xilinx_env = ":xilinx_env.sh",
    deps = [":hls_adder"],
)

vitis_generate(
    name = "adder_vivado",
    out = "adder_vivado.tar.gz",
    clock_period = "10.0",
    tags = ["manual"],
    # Note need namespace with vivado_hls.
    top_func = "vitis::adder",
    use_vivado_hls = True,
    xilinx_env = ":xilinx_env_vivado.sh",
    deps = [":hls_adder_vivado"],
)
