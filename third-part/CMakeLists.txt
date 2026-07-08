project(third-part)

add_subdirectory(tomlplusplus)

add_library(fast_csv INTERFACE)
target_include_directories(fast_csv
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/fast-cpp-csv-parser
)
target_compile_features(fast_csv
    INTERFACE
        cxx_std_11
)

set(SPDLOG_BUILD_SHARED ON CACHE BOOL "Build spdlog as shared library" FORCE)
add_subdirectory(spdlog)
