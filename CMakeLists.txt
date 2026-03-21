cmake_minimum_required(VERSION 3.13)

project(tshell)

set(SOURCE_FILES
    src/main.cpp
    src/reader.cpp
    src/lexer.cpp
    src/parse.cpp
    src/execute.cpp
    src/commands.cpp
    src/redirect.cpp
    src/executable_cache.cpp
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(shell ${SOURCE_FILES})

target_compile_options(shell PRIVATE -Wall -Wextra -Wpedantic)

find_package(PkgConfig REQUIRED)
pkg_check_modules(READLINE REQUIRED readline)
target_link_libraries(shell PRIVATE ${READLINE_LIBRARIES})

target_include_directories(shell PRIVATE ${PROJECT_SOURCE_DIR}/include)
