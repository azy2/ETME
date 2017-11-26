cmake_minimum_required(VERSION 3.9)

project(etme)

set(CMAKE_CXX_COMPILER /usr/bin/clang++-5.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wextra -pedantic -Werror -Wall")

set(CORE_INCLUDE_DIR core/include)
set(CORE_HEADERS core/include/Buffer.h core/include/BufferView.h core/include/Cursor.h core/include/Optional.h)
set(CORE_SRCS core/src/Buffer.cpp core/src/BufferView.cpp)

add_library(etme_core ${CORE_SRCS} ${CORE_HEADERS})
target_include_directories(etme_core PRIVATE ${CORE_INCLUDE_DIR})

set(CORE_TEST_SRCS core/test/test_main.cpp core/test/BufferTest.cpp)

add_executable(etme_test ${CORE_TEST_SRCS})
target_link_libraries(etme_test etme_core)
target_include_directories(etme_test PRIVATE ${CORE_INCLUDE_DIR})

#find_package(Curses REQUIRED)
#find_package(Threads REQUIRED)
#add_executable(etme_ncurses ncurses/ncurses.cpp ncurses/Keybindings.cpp ${SOURCES})
#target_include_directories(etme_ncurses PRIVATE ${CURSES_INCLUDE_DIR})
#target_link_libraries(etme_ncurses ${CURSES_LIBRARIES})
#target_link_libraries(etme_ncurses ${CMAKE_THREAD_LIBS_INIT})

find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM3 gtkmm-3.0 REQUIRED)
add_executable(etme_gtk gtk/gtk.cpp)
target_include_directories(etme_gtk PRIVATE ${GTKMM3_INCLUDE_DIRS})
target_include_directories(etme_gtk PRIVATE ${CORE_INCLUDE_DIR})
target_link_libraries(etme_gtk etme_core)
target_link_libraries(etme_gtk ${GTKMM3_LIBRARIES})
