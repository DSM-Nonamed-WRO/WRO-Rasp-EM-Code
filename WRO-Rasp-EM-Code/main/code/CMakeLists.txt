cmake_minimum_required(VERSION 3.10)
project(WRO_Rasp_Project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV 4 REQUIRED)

add_subdirectory(components/socket_server)

add_executable(
    wro
    main.cpp
)

target_link_libraries(
    wro
    socket_server
    ${OpenCV_LIBS}
)

target_include_directories(
    wro
    PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)
