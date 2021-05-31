cmake_minimum_required(VERSION 3.16.3)
project(solveur)

find_package(Curses REQUIRED)

file(COPY grid_old.txt DESTINATION .)
file(COPY waves.txt DESTINATION .)

add_executable(solveur src/main.c src/fonctions.h src/TerminalUtils.c src/simulation.c src/stack.c src/stack.h src/simulation_manager.c src/fonctions.c)
target_link_libraries(solveur ${CURSES_LIBRARIES})
target_link_libraries(solveur m)