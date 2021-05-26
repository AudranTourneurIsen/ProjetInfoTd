cmake_minimum_required(VERSION 3.19)
project(solveur)

find_package(Curses REQUIRED)

file(COPY grid.txt DESTINATION .)

add_executable(solveur src/main.c src/fonctions.h src/TerminalUtils.c src/simulation.c)
target_link_libraries(solveur ${CURSES_LIBRARIES})
