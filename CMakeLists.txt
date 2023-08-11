cmake_minimum_required(VERSION 3.10)

project(sr_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROTO_FILE output.proto)

find_package(Protobuf REQUIRED)


protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})
# # # protobuf_generate(TARGET ${PROJECT_NAME} LANGUAGE cpp PROTOS ${PROTO_FILE})

# add_executable(output output.pb.cc ${PROTO_SRCS} ${PROTO_HDRS})
add_library(output ${PROTO_SRCS} ${PROTO_HDRS})
# target_link_libraries(output PUBLIC ${Protobuf_LIBRARIES})


add_executable(${PROJECT_NAME} src/main.cpp src/tcp_client.cpp src/tcp_server.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} include)
target_link_libraries(${PROJECT_NAME} output ${Protobuf_LIBRARIES})     