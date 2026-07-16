project(snippet_tests)

add_executable(test_csv
    test_csv.cpp
    ${CMAKE_SOURCE_DIR}/src/fast-cpp-csv-parser/fast-cpp-csv-parser.cpp
)
target_include_directories(test_csv BEFORE PRIVATE
    ${CMAKE_SOURCE_DIR}/third-part/spdlog/include
)
target_link_libraries(test_csv PRIVATE Catch2::Catch2WithMain fast_csv spdlog::spdlog)
target_include_directories(test_csv PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_csv COMMAND test_csv)

add_executable(test_config
    test_config.cpp
    ${CMAKE_SOURCE_DIR}/src/tomlplusplus/tomlplusplus.cpp
)
target_include_directories(test_config BEFORE PRIVATE
    ${CMAKE_SOURCE_DIR}/third-part/spdlog/include
)
target_link_libraries(test_config PRIVATE Catch2::Catch2WithMain tomlplusplus_tomlplusplus spdlog::spdlog)
target_include_directories(test_config PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_config COMMAND test_config)
