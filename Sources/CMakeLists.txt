cmake_minimum_required(VERSION 3.12)

set(THIS Sources)

set(HEADERS
    SharedConditionVariable.h
    SharedMemory.h
    SharedMutex.h
    Tool.h
)

set(SOURCES
    SharedConditionVariable.cpp
    SharedMemory.cpp
    SharedMutex.cpp
    Tool.cpp
)

add_library(${THIS} STATIC ${HEADERS} ${SOURCES})