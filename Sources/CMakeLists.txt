cmake_minimum_required(VERSION 3.12)

set(THIS Sources)

set(HEADERS
    SharedSemaphore.h
    SharedMemory.h
    Tool.h
)

set(SOURCES
    SharedSemaphore.cpp
    SharedMemory.cpp
    Tool.cpp
)

add_library(${THIS} STATIC ${HEADERS} ${SOURCES})