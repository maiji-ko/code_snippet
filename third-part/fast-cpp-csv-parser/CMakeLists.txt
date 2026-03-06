add_library(fast_csv INTERFACE)

target_include_directories(fast_csv
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(fast_csv
    INTERFACE
        cxx_std_11
)