cmake_minimum_required(VERSION 3.13)

project(tshell)

set(SOURCE_FILES
    src/main.cpp
    src/lexer.cpp
    src/parse.cpp
    src/execute.cpp
    src/commands.cpp
    src/utils.cpp
)

set(CMAKE_CXX_STANDARD 23)

add_executable(shell ${SOURCE_FILES})

target_include_directories(shell
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)
