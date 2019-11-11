cmake_minimum_required(VERSION 3.15)
project(Orga6620_TP2_Cache C)

set(CMAKE_C_STANDARD 99)

add_executable(Orga6620_TP2_Cache main.c cache.c cache_constants.h cache.h fileReader.c fileReader.h)