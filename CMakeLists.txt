cmake_minimum_required(VERSION 3.14)

project(crisps-8
        VERSION 1.0
        DESCRIPTION "CHIP-8 interpreter"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)
