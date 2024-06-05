project(myproto VERSION 0.0.1 LANGUAGES CXX)

find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    myproto/address.proto
    myproto/addressbook.proto
)

target_compile_features(${PROJECT_NAME}
  PRIVATE
  cxx_std_20
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

get_target_property(grpc_cpp_plugin_location
    gRPC::grpc_cpp_plugin
    LOCATION
)

protobuf_generate(TARGET ${PROJECT_NAME}
    LANGUAGE cpp
)

protobuf_generate(TARGET ${PROJECT_NAME}
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)
