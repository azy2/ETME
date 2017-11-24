cmake_minimum_required(VERSION 3.9)

project(textedit)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "-g -Og")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(CAF COMPONENTS core io REQUIRED)
find_package(Curses REQUIRED)

include_directories(${CAF_INCLUDE_DIRS})
include_directories(${CURSES_INCLUDE_DIR})

add_executable(textedit src/main.cpp src/io.cpp src/buffer.cpp)
target_link_libraries(textedit ${CAF_LIBRARIES})
target_link_libraries(textedit ${CURSES_LIBRARIES})
