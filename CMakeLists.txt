cmake_minimum_required(VERSION 3.22)

project(gRPC_CMake VERSION 0.0.1 LANGUAGES CXX)

add_subdirectory(proto)
add_subdirectory(server)
add_subdirectory(client)
