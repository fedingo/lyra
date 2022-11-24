cc_library(
    name="iotypes",
    hdrs = [
        "iotypes.h"
    ],
    srcs = [
        "iotypes.cpp"
    ]
)

cc_binary(
    name = "encoder",
    srcs = [
        "encoder_binding.cpp",
    ],
    deps = [
        ":iotypes",
        "//:lyra_config",
        "//:lyra_encoder",
        "@com_google_absl//absl/types:span",
        "@com_google_glog//:glog",
        "@gulrak_filesystem//:filesystem",
    ],
    visibility = ["//visibility:public"],
    linkstatic=True,
    linkshared = True,
)

cc_binary(
    name = "decoder",
    srcs = [
        "decoder_binding.cpp",
    ],
    deps = [
        ":iotypes",
        "//:lyra_config",
        "//:lyra_decoder",
        "@com_google_absl//absl/types:span",
        "@com_google_glog//:glog",
        "@gulrak_filesystem//:filesystem",
    ],
    visibility = ["//visibility:public"],
    linkstatic=True,
    linkshared = True,
)