cmake_minimum_required(VERSION 3.31)
project(bet_game)

enable_testing()

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# protobuf configuration
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/proto/bet_game/v1/server_msg.proto)

add_library(proto
        ${PROTO_SRCS}
)
target_link_libraries(proto ${Protobuf_LIBRARIES})

add_executable(server ${CMAKE_CURRENT_SOURCE_DIR}/src/server.cpp)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(server proto )

add_executable(client ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp)
target_include_directories(client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(client proto)

include(GoogleTest)
find_package(GTest REQUIRED)

add_executable(BetTest
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp
)
target_link_libraries(BetTest GTest::gtest GTest::gtest_main proto)

gtest_discover_tests(BetTest)
