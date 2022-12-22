include(FetchContent)

FetchContent_Declare(
    SDL2
    URL         https://github.com/libsdl-org/SDL/releases/download/release-2.24.2/SDL2-2.24.2.zip
    SOURCE_DIR  ${CMAKE_SOURCE_DIR}/dependencies/SDL2
)
message(STATUS "Fetching SDL2...")
FetchContent_MakeAvailable(SDL2)
