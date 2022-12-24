include(FetchContent)
set(FETCHCONTENT_QUIET false)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
    GIT_TAG         release-2.24.1
    SOURCE_DIR      ${CMAKE_SOURCE_DIR}/dependencies/SDL2
)
message(STATUS "Fetching SDL2...")
FetchContent_MakeAvailable(SDL2)
