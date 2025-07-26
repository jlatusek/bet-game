cmake_minimum_required(VERSION 3.31)
project(bet_game)

enable_testing()


set(CMAKE_CXX_STANDARD 26)

# protobuf configuration
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/proto/bet_game/v1/server_msg.proto)

add_library(io
        ${CMAKE_CURRENT_SOURCE_DIR}/src/io/socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/io/file.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/epoll.cpp
        ${PROTO_SRCS}
)
target_include_directories(io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(io ${Protobuf_LIBRARIES})

add_library(event
        ${CMAKE_CURRENT_SOURCE_DIR}/src/event/event.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/event/event_handler.cpp
)
target_include_directories(event PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(threading
        ${CMAKE_CURRENT_SOURCE_DIR}/src/threading/thread_pool.cpp
)
target_include_directories(threading PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(bet-server ${CMAKE_CURRENT_SOURCE_DIR}/src/server.cpp)
target_include_directories(bet-server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(bet-server io threading event)

add_executable(bet-client ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp)
target_include_directories(bet-client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(bet-client io)

include(GoogleTest)
find_package(GTest REQUIRED)

add_executable(BetTest
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/queue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/messages.cpp
)
target_link_libraries(BetTest GTest::gtest GTest::gtest_main io)

gtest_discover_tests(BetTest)
