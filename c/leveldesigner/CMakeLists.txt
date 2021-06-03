cmake_minimum_required(VERSION 3.10)

project(leveldesigner)

FILE(GLOB Sources src/*.c)
add_executable(leveldesigner.exe ${Sources})