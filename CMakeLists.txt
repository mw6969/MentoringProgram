cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME MentoringProgram)

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

set(Sources main.cpp)

add_executable(${PROJECT_NAME} ${Sources})