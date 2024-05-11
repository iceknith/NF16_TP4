cmake_minimum_required(VERSION 3.27)
project(NF16_TP4 C)

set(CMAKE_C_STANDARD 11)

add_executable(NF16_TP4 main.c)
add_library(TP4 tp4.c tp4.h)