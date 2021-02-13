cmake_minimum_required(VERSION 3.10)
project(spde-server)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(GRPC REQUIRED)
find_package(Protobuf REQUIRED)

set(PROTOS
        proto/spde.proto
        )

grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTOS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOS})

set(HDRS
        include/sweep.hpp
        include/spde_server.hpp
        )

set(SRCS
        src/sweep.cpp
        src/spde_server.cpp
        )

include_directories(
        include
        ${CURL_INCLUDE_DIR}
        ${GRPC_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${PROJECT_NAME}
        main.cpp
        ${HDRS}
        ${SRCS}
        ${GRPC_HDRS}
        ${GRPC_SRCS}
        ${PROTO_HDRS}
        ${PROTO_SRCS}
        )

add_executable(spde-client
        main_client.cpp
        src/spde_client.cpp
        include/spde_client.hpp
        ${GRPC_HDRS}
        ${GRPC_SRCS}
        ${PROTO_HDRS}
        ${PROTO_SRCS}
        )

target_link_libraries(${PROJECT_NAME}
        pthread
        ${GRPC_LIBRARIES}
        ${PROTOBUF_LIBRARIES}
        )

target_link_libraries(spde-client
        pthread
        ${GRPC_LIBRARIES}
        ${PROTOBUF_LIBRARIES}
        )