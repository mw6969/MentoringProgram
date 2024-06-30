cmake_minimum_required(VERSION 3.12)
project(MentoringProgram CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "src/*.cpp")

find_package(Boost 1.81.0 REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
find_package(CryptoPP REQUIRED)
if(Boost_FOUND AND CRYPTOPP_FOUND)
  include_directories(include ${Boost_INCLUDE_DIRS} ${CRYPTOPP_INCLUDE_DIR})
  add_executable(app ${SOURCES})
  target_link_libraries(app ${Boost_LIBRARIES} ${CRYPTOPP_LIBRARIES})
endif()