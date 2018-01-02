cmake_minimum_required(VERSION 3.9)
project(game C)

set(CMAKE_C_STANDARD 99)

add_executable(game main.c classes.c classes.h functions.c functions.h config.h)

target_link_libraries(game SDL2main SDL2 SDL2_image)