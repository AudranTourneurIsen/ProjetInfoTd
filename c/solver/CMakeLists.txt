cmake_minimum_required(VERSION 3.10)
project(solver)

find_package(Curses REQUIRED)

add_compile_options(-Wall -Wextra -Werror)

FILE(GLOB Sources src/*.c)
add_executable(solver.exe ${Sources})


target_link_libraries(solver.exe ${CURSES_LIBRARIES})
target_link_libraries(solver.exe m)