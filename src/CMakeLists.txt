file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*)

add_executable(crisps-8 ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(crisps-8 PRIVATE SDL2)
target_include_directories(crisps-8
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(crisps-8 
    PUBLIC
    SDL2::SDL2
    SDL2::SDL2main
)
