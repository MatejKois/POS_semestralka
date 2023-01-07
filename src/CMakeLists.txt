cmake_minimum_required(VERSION 3.18)
project(POS_semestralka C)

set(CMAKE_C_STANDARD 99)

add_executable(POS_semestralka main.c Canvas/canvas.c Canvas/canvas.h Simulation/simulation.h Simulation/simulation.c Menu/menu.h Menu/menu.c Communication/client.h Communication/client.c Communication/server.h Communication/server.c)
target_link_libraries(POS_semestralka pthread)
