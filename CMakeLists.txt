cmake_minimum_required(VERSION 3.11)

project(crisps-8 
        VERSION 1.0
        DESCRIPTION "CHIP-8 interpreter"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)              #Sets c++ standard to c++ 14
set(CMAKE_CXX_STANDARD_REQUIRED True)   #Forces that standard

set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
target_include_directories(${PROJECT_NAME} PRIVATE include/)

# Add SDL2 library
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Main)
