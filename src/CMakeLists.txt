project(snippet_executable)

add_executable(${PROJECT_NAME}
    tomlplusplus/tomlplusplus.cpp
    fast-cpp-csv-parser/fast-cpp-csv-parser.cpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        fast_csv
        tomlplusplus_tomlplusplus
        spdlog::spdlog
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_RPATH    "${CMAKE_BINARY_DIR}/third-part/spdlog"
    INSTALL_RPATH  "$ORIGIN/../lib"
)