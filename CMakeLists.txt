cmake_minimum_required(VERSION 3.30)
project(gestion_reservas_teatro C)

set(CMAKE_C_STANDARD 11)

add_executable(gestion_reservas_teatro main.c sala.c sala.h)
