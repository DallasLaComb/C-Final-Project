cmake_minimum_required(VERSION 3.5)
project(snake_game C)

set(CMAKE_C_STANDARD 11)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})

# Specify the SDL2_ttf paths manually
include_directories("/usr/include/SDL2")
link_directories("/usr/lib/x86_64-linux-gnu")

# Add the executable
add_executable(snake_game snake_game.c)

# Link SDL2 and SDL2_ttf libraries explicitly
target_link_libraries(snake_game ${SDL2_LIBRARIES} SDL2_ttf)
