cmake_minimum_required (VERSION 3.8)

project ("gameoflife")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable (gameoflife src/main.cpp src/grid.cpp)
target_compile_features(gameoflife PUBLIC cxx_std_20)

target_link_libraries(gameoflife PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_include_directories(gameoflife PRIVATE ${SDL2_INCLUDE_DIRS})