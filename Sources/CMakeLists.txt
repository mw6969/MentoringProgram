cmake_minimum_required(VERSION 3.12)

set(THIS Sources)

set(HEADERS
    SharedMemory.h
    Tool.h
)

set(SOURCES
    SharedMemory.cpp
    Tool.cpp
)

add_library(${THIS} STATIC ${HEADERS} ${SOURCES})