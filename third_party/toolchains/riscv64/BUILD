package(default_visibility = ["//visibility:public"])

load(":cc_config.bzl", "cc_toolchain_config")

cc_toolchain_suite(
    name = "toolchain",
    toolchains = {
        "riscv64": ":cc-compiler-riscv64",
    },
)

filegroup(
    name = "empty",
    srcs = [],
)

filegroup(
    name = "riscv64_linux_all_files",
    srcs = [
        "@riscv64_compiler//:compiler_pieces",
    ],
)

cc_toolchain_config(
    name = "riscv64_config",
    cpu = "riscv64",
)

cc_toolchain(
    name = "cc-compiler-riscv64",
    all_files = ":riscv64_linux_all_files",
    compiler_files = ":riscv64_linux_all_files",
    dwp_files = ":empty",
    linker_files = ":riscv64_linux_all_files",
    objcopy_files = "riscv64_linux_all_files",
    strip_files = "riscv64_linux_all_files",
    supports_param_files = 1,
    toolchain_config = ":riscv64_config",
    toolchain_identifier = "riscv64-linux-gnu",
)
