cmake_minimum_required(VERSION 3.10)

project(sr_test)

find_package(Protobuf REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROTO_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/output.proto)

get_filename_component(FILE_NAME ${PROTO_FILE_NAME} NAME_WE)

set(PYTHON_PATH_OUT ${CMAKE_CURRENT_SOURCE_DIR}/python_client/)
set(CPP_PATH_OUT ${CMAKE_CURRENT_BINARY_DIR}/)

set(ARGS "-I=${CMAKE_CURRENT_SOURCE_DIR} --python_out=${PYTHON_PATH_OUT} --cpp_out=${CPP_PATH_OUT} --proto_path=${Protobuf_INCLUDE_DIRS}")
set(EXECUTE_PROTOBUF_COMMAND "${Protobuf_PROTOC_EXECUTABLE} ${ARGS} ${PROTO_FILE_NAME}")
EXEC_PROGRAM(${EXECUTE_PROTOBUF_COMMAND})

set(PROTO_SRCS ${CPP_PATH_OUT}/${FILE_NAME}.pb.cc)
set(PROTO_HDRS ${CPP_PATH_OUT}/${FILE_NAME}.pb.h)

# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})
# add_library(output ${PROTO_SRCS} ${PROTO_HDRS})

add_executable(client src/client_main.cpp src/tcp_client.cpp ${PROTO_SRCS} ${PROTO_HDRS} )
target_include_directories(client PUBLIC ${CMAKE_CURRENT_BINARY_DIR} include)
target_link_libraries(client ${Protobuf_LIBRARIES})     

add_executable(server src/server_main.cpp src/tcp_server.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(server PUBLIC ${CMAKE_CURRENT_BINARY_DIR} include)
target_link_libraries(server ${Protobuf_LIBRARIES}) 
 