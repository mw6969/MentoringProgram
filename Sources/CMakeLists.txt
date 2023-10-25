cmake_minimum_required(VERSION 3.12)

set(THIS Sources)

set(HEADERS
    FileValidator.h
    Tool.h
)

set(SOURCES
    FileValidator.cpp
    Tool.cpp
)

add_library(${THIS} STATIC ${HEADERS} ${SOURCES})