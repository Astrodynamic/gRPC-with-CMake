project(server VERSION 0.0.1 LANGUAGES CXX)

set(HEADERS
    src/control_impl.h
    src/server.h
)

set(SOURCES
    src/control_impl.cc
    src/server.cc
)

add_executable(${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
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
