cmake_minimum_required(VERSION 3.9)

project(etme)

set(CMAKE_CXX_COMPILER /usr/bin/clang++-5.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wextra -pedantic -Werror -Wall")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

set(SRCDIR src)
set(SOURCES ${SRCDIR}/io.cpp ${SRCDIR}/buffer.cpp)

add_executable(etme_ncurses ncurses/ncurses.cpp ${SOURCES})
target_link_libraries(etme_ncurses ${CURSES_LIBRARIES})
target_link_libraries(etme_ncurses ${CMAKE_THREAD_LIBS_INIT})
