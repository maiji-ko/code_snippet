cmake_minimum_required(VERSION 3.30.5)

project(snippet)

add_subdirectory(third-part/fast-cpp-csv-parser)

include_directories(src)

add_executable(${PROJECT_NAME}
    src/fast-cpp-csv-parser/fast-cpp-csv-parser.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE fast_csv)