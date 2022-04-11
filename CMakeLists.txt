cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_STANDARD 17)
project(BookSystem C CXX)

include(./grpc_common.cmake)

# Proto file
get_filename_component(bs_proto "book_system.proto" ABSOLUTE)
get_filename_component(bs_proto_path "${bs_proto}" PATH)
# Generated sources
set(bs_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/book_system.pb.cc")
set(bs_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/book_system.pb.h")
set(bs_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/book_system.grpc.pb.cc")
set(bs_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/book_system.grpc.pb.h")
add_custom_command(
        OUTPUT "${bs_proto_srcs}" "${bs_proto_hdrs}" "${bs_grpc_srcs}" "${bs_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${bs_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${bs_proto}"
        DEPENDS "${bs_proto}")

# include generated *_sql.h files
include_directories("cpp_sql")
# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
# bs_grpc_proto
add_library(bs_grpc_proto
        ${bs_grpc_srcs}
        ${bs_grpc_hdrs}
        ${bs_proto_srcs}
        ${bs_proto_hdrs})
target_link_libraries(bs_grpc_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

# Targets book_system_(client|server)
foreach (_target
        book_system_client book_system_server)
    add_executable(${_target}
            "${_target}.cpp")
    target_link_libraries(${_target}
            bs_grpc_proto
            ${_REFLECTION}
            ${_GRPC_GRPCPP}
            ${_PROTOBUF_LIBPROTOBUF}
            mysqlcppconn
            mysqlclient
            )
endforeach ()
