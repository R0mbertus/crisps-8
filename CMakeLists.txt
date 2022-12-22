cmake_minimum_required(VERSION 3.14)

project(crisps-8
        VERSION 1.0
        DESCRIPTION "CHIP-8 interpreter"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif (CCACHE_FOUND)

add_subdirectory(dependencies)
include_directories(include)
add_subdirectory(src)
