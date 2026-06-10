add_librarry(
  socket
  socket_server.cpp
)

target_include_directories(
  socket
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
