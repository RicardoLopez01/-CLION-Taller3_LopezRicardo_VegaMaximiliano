cmake_minimum_required(VERSION 3.29)
project(Taller3_LopezRicardo_VegaMaximiliano)

set(CMAKE_CXX_STANDARD 20)

# Incluir la carpeta de encabezados
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Crear el ejecutable
add_executable(Taller3_LopezRicardo_VegaMaximiliano main.cpp
        cmake-build-debug/include/stb_image.h
        stb_image_resize2.h)