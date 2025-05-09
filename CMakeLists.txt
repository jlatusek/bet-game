cmake_minimum_required(VERSION 3.31)
project(bet_game)

set(CMAKE_CXX_STANDARD 26)

add_library(io ${CMAKE_CURRENT_SOURCE_DIR}/src/io/socket.cpp)
target_include_directories(io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/io)

add_executable(server ${CMAKE_CURRENT_SOURCE_DIR}/src/server.cpp)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(server io)

add_executable(client ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp)
target_include_directories(client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(client io)