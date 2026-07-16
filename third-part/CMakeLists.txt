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

set(CATCH_BUILD_TESTING OFF CACHE BOOL "Disable Catch2's own tests" FORCE)
set(CATCH_INSTALL_DOCS OFF CACHE BOOL "Disable Catch2 doc installation" FORCE)
add_subdirectory(catch2)
