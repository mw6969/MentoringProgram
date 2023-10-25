cmake_minimum_required(VERSION 3.12)

set(THIS MentoringProgram)

project(${THIS} CXX)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    main.cpp
)

add_executable(${THIS} ${SOURCES})

target_include_directories(${THIS} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Sources)
target_link_libraries(${THIS} Sources)

add_subdirectory(Sources)