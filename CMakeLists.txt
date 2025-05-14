cmake_minimum_required(VERSION 3.31)
project(bet_game)

enable_testing()


set(CMAKE_CXX_STANDARD 26)

add_library(io ${CMAKE_CURRENT_SOURCE_DIR}/src/io/socket.cpp)
target_include_directories(io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(server ${CMAKE_CURRENT_SOURCE_DIR}/src/server.cpp)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(server io)

add_executable(client ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp)
target_include_directories(client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(client io)

include(GoogleTest)
find_package(GTest REQUIRED)

add_executable(BetTest
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/queue.cpp
)
target_link_libraries(BetTest GTest::gtest GTest::gtest_main io)

gtest_discover_tests(BetTest)
