cmake_minimum_required(VERSION 3.12)
project(MentoringProgram CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.81.0 REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
find_package(CryptoPP REQUIRED)
if(Boost_FOUND AND CRYPTOPP_FOUND)
  file(GLOB PUBLIC_SOURCES "src/*.cpp")
  file(GLOB SERVER_SOURCES "src/server/*.cpp")
  file(GLOB CLIENT_SOURCES "src/client/*.cpp")

  include_directories(include include/server include/client ${Boost_INCLUDE_DIRS} ${CRYPTOPP_INCLUDE_DIR})

  add_executable(server ${PUBLIC_SOURCES} ${SERVER_SOURCES})
  target_link_libraries(server ${Boost_LIBRARIES} ${CRYPTOPP_LIBRARIES})

  add_executable(client ${PUBLIC_SOURCES} ${CLIENT_SOURCES})
  target_link_libraries(client ${Boost_LIBRARIES} ${CRYPTOPP_LIBRARIES})
endif()