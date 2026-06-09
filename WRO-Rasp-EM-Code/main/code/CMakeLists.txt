cmake_minimum_required(VERSION 3.10)
project(WRO_Rasp_Project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV 4 REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/components/socket_server
    ${CMAKE_CURRENT_SOURCE_DIR}/components/color_detector
)

set(SOURCE_FILES
    main.cpp
    components/socket_server/socket_server.cpp
    components/color_detector/color_detector.cpp
)

add_executable(wro ${SOURCE_FILES})

target_link_libraries(wro ${OpenCV_LIBS})
