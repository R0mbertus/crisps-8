cmake_minimum_required(VERSION 3.11)

project(crisps-8 
        VERSION 1.0
        DESCRIPTION "CHIP-8 interpreter"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)              #Sets c++ standard to c++ 14
set(CMAKE_CXX_STANDARD_REQUIRED True)   #Forces that standard

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(crisps-8 src/main.cpp src/sdl2_init.cpp src/chip_8.cpp)
target_compile_options(crisps-8 PRIVATE -Wall -Wextra)
target_include_directories(crisps-8 PRIVATE include/)
target_link_libraries(crisps-8 ${SDL2_LIBRARIES})
