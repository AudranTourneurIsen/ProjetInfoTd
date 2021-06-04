cmake_minimum_required(VERSION 3.10)
project(legacysolver)

find_package(Curses REQUIRED)

add_compile_options(-Wall -Wextra -Werror)

FILE(GLOB Sources src/*.c)
add_executable(legacysolver.exe ${Sources})


target_link_libraries(legacysolver.exe ${CURSES_LIBRARIES})
target_link_libraries(legacysolver.exe m)