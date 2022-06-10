cmake_minimum_required(VERSION 3.22)
project(raytracing)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(raytracing
        bmp.h
        common.h
        geometry.h
        main.cpp
        scene.h
        shading.h render.h light.h AntiAliasing.h)
