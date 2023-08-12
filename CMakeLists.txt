cmake_minimum_required(VERSION 3.10)

project(sr_test)

set(Protobuf_PROTOC_EXECUTABLE "/usr/bin/protoc")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROTO_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/output.proto)

find_package(Protobuf REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(GTest REQUIRED)

get_filename_component(FILE_NAME ${PROTO_FILE_NAME} NAME_WE)

set(PYTHON_PATH_OUT ${CMAKE_CURRENT_SOURCE_DIR}/python_client/)
set(CPP_PATH_OUT ${CMAKE_CURRENT_BINARY_DIR}/)

set(ARGS "-I=${CMAKE_CURRENT_SOURCE_DIR} --python_out=${PYTHON_PATH_OUT} --cpp_out=${CPP_PATH_OUT} --proto_path=${Protobuf_INCLUDE_DIRS}")
set(EXECUTE_PROTOBUF_COMMAND "${Protobuf_PROTOC_EXECUTABLE} ${ARGS} ${PROTO_FILE_NAME}")
EXEC_PROGRAM(${EXECUTE_PROTOBUF_COMMAND})

set(PROTO_SRCS ${CPP_PATH_OUT}/${FILE_NAME}.pb.cc)
set(PROTO_HDRS ${CPP_PATH_OUT}/${FILE_NAME}.pb.h)
enable_testing()

function(add_my_executable TARGET_NAME SOURCES USE_GTEST)
    add_executable(${TARGET_NAME} ${SOURCES} ${PROTO_SRCS} ${PROTO_HDRS} )
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} include)
    target_link_libraries(${TARGET_NAME} ${Protobuf_LIBRARIES} yaml-cpp)
    if(USE_GTEST)
        target_link_libraries(${TARGET_NAME} ${GTEST_BOTH_LIBRARIES})
    endif()
endfunction()

add_my_executable(client "src/client_main.cpp;src/tcp_client.cpp" False)
add_my_executable(server "src/server_main.cpp;src/tcp_server.cpp" False)
add_my_executable(test_tcp_server "test/test_tcp_server.cpp;src/tcp_server.cpp" True)
add_my_executable(test_tcp_client "test/test_tcp_client.cpp;src/tcp_client.cpp" True)

include(GoogleTest)
gtest_discover_tests(test_tcp_client)
gtest_discover_tests(test_tcp_server)