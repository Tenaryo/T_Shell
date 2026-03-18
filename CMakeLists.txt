cmake_minimum_required(VERSION 3.13)

project(tshell)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

set(CMAKE_CXX_STANDARD 23)

add_executable(shell ${SOURCE_FILES})

target_include_directories(shell
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)
