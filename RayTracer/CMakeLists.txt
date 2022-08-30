cmake_minimum_required(VERSION 3.22)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracer main.cpp vec3.h ray.h hitable.h sphere.h hitablelist.h camera.h material.h random.h)
