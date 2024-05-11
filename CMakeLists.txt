cmake_minimum_required(VERSION 3.27)
project(NF16_TP4 C)
set(CMAKE_C_STANDARD 11)

# On utilise le module PkgConfig pour détecter la librairie GTK+ sur le système
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

# On dit à CMake d'utiliser GTK+, on indique au compilateur où trouver les fichiers headers
# Et au linker où trouver les librairies
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Instructions pour le compilateur
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

#Pour linux, va servir à correctement liers les signaux de notre application au code
if (UNIX)
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-export-dynamic")
endif (UNIX)

add_executable(NF16_TP4 main.c main.h)
add_library(NF16_TP4_tp4 tp4.c tp4.h)
add_library(NF16_TP4_graphicalInterface graphicalInterface.c graphicalInterface.h)

target_link_libraries(NF16_TP4 ${GTK3_LIBRARIES})
