cmake_minimum_required(VERSION 3.9)
project(arcshoot C)

set(CMAKE_C_STANDARD 99)

add_executable(arcshoot main.c classes.c classes.h config.h window.c window.h enemy.c player.c)

target_link_libraries(arcshoot SDL2main SDL2 SDL2_image)