cmake_minimum_required(VERSION 3.12)

set(THIS Sources)

set(HEADERS
    Tool.h
)

set(SOURCES
    Tool.cpp
)

add_library(${THIS} STATIC ${HEADERS} ${SOURCES})