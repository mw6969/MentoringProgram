cmake_minimum_required(VERSION 3.12)
project(MentoringProgram CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "src/*.cpp")

add_executable(app ${SOURCES})