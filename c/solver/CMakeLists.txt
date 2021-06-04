cmake_minimum_required(VERSION 3.10)
project(solver)

add_compile_options(-Wall -Wextra -Werror)

FILE(GLOB Sources src/*.c)
add_executable(solver.exe ${Sources})

target_link_libraries(solver.exe m)