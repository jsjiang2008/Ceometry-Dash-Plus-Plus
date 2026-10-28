cmake_minimum_required(VERSION 3.21)

project(CeometryDash LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3)

add_executable(ceometry-dash
    src/app/main.cpp
)

target_link_libraries(
    ceometry-dash PRIVATE SDL3::SDL3
)
