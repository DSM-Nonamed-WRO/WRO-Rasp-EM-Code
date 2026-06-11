add_library(
    socket_client
    socket_client.cpp
)

target_include_directories(
    socket_client
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
