cmake_minimum_required(VERSION 3.14)

project(Allocator VERSION 1.0 DESCRIPTION "memory management" LANGUAGES C)

set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
include_directories(src)

file(GLOB SOURCES_C "src/*.c")
file(GLOB SOURCES_H "include/alloc.h")

add_executable(alloc ${SOURCES_C} ${SOURCES_H})
