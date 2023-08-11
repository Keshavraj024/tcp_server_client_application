cmake_minimum_required(VERSION 3.10)

project(sr_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROTO_FILE output.proto)

find_package(Protobuf REQUIRED)


protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})

add_library(output ${PROTO_SRCS} ${PROTO_HDRS})

add_executable(client src/client_main.cpp src/tcp_client.cpp )
target_include_directories(client PUBLIC ${CMAKE_CURRENT_BINARY_DIR} include)
target_link_libraries(client output ${Protobuf_LIBRARIES})     

add_executable(server src/server_main.cpp src/tcp_server.cpp)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_BINARY_DIR} include)
target_link_libraries(server output ${Protobuf_LIBRARIES})  