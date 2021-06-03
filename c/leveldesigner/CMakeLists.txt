cmake_minimum_required(VERSION 3.10)

project(leveldesigner)

add_compile_options(-Wall -Wextra -pedantic -Werror)

FILE(GLOB Sources src/*.c)
add_executable(leveldesigner.exe ${Sources})