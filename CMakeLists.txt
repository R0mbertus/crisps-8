cmake_minimum_required(VERSION 3.11)

project(crisps-8
        VERSION 1.0
        DESCRIPTION "CHIP-8 interpreter"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

file(GLOB SOURCES "src/*.cpp")
add_executable(crisps-8 ${SOURCES})

target_compile_options(crisps-8 PRIVATE -Wall -Wextra )
target_include_directories(crisps-8 PRIVATE include/)

# Add SDL2 library
find_package(SDL2 REQUIRED)
target_link_libraries(crisps-8 SDL2::Main)
