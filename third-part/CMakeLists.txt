project(third-part)

# toml
add_subdirectory(tomlplusplus-3.4.0)

# csv (auto-downloaded via CMake FetchContent)
include(FetchContent)

FetchContent_Declare(
    fast-cpp-csv-parser
    GIT_REPOSITORY https://github.com/ben-strasser/fast-cpp-csv-parser.git
    GIT_TAG        origin/master
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