project(server VERSION 0.0.1 LANGUAGES CXX)

set(HEADERS
    src/async_control_impl.h
    src/sync_control_impl.h
    src/callback_control_impl.h
)

set(SOURCES
    src/async_control_impl.cc
    src/sync_control_impl.cc
    src/callback_control_impl.cc
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
