project(snippet_executable)

add_executable(${PROJECT_NAME}
    fast-cpp-csv-parser/fast-cpp-csv-parser.cpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        fast_csv
        tomlplusplus_tomlplusplus
)