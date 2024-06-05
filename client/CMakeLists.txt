project(client VERSION 0.0.1 LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    src/main.cc
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
    cxx_std_20
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    myproto
)
