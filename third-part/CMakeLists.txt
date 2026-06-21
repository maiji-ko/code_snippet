project(third-part)

include(FetchContent)

# toml (auto-downloaded via CMake FetchContent)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        origin/master
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(tomlplusplus)

# csv (auto-downloaded via CMake FetchContent)
FetchContent_Declare(
    fast-cpp-csv-parser
    GIT_REPOSITORY https://github.com/ben-strasser/fast-cpp-csv-parser.git
    GIT_TAG        origin/master
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(fast-cpp-csv-parser)

add_library(fast_csv INTERFACE)
target_include_directories(fast_csv
    INTERFACE
        ${fast-cpp-csv-parser_SOURCE_DIR}
)
target_compile_features(fast_csv
    INTERFACE
        cxx_std_11
)

# spdlog (auto-downloaded via CMake FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.17.0
    GIT_SHALLOW    TRUE
)
set(SPDLOG_BUILD_SHARED ON CACHE BOOL "Build spdlog as shared library" FORCE)

FetchContent_MakeAvailable(spdlog)