cmake_minimum_required(VERSION 3.20.1)

set(CMAKE_CXX_COMPILER clang++)

project(snippet)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_subdirectory(third-part)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
