find_package(OpenCV 4 REQUIRED)

add_library(
  socket_server
  socket_server.cpp
)

target_include_directories(
  socket_server
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(
  socket_server
  PUBLIC 
  ${OpenCV_LIBS}
)
