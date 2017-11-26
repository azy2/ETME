cmake_minimum_required(VERSION 3.9)

project(etme)

set(CMAKE_CXX_COMPILER /usr/bin/clang++-5.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wextra -pedantic -Werror -Wall")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(SRCDIR src)
set(SOURCES ${SRCDIR}/BufferView.cpp ${SRCDIR}/Buffer.cpp src/Optional.h)


#find_package(Curses REQUIRED)
#find_package(Threads REQUIRED)
#add_executable(etme_ncurses ncurses/ncurses.cpp ncurses/Keybindings.cpp ${SOURCES})
#target_include_directories(etme_ncurses PRIVATE ${CURSES_INCLUDE_DIR})
#target_link_libraries(etme_ncurses ${CURSES_LIBRARIES})
#target_link_libraries(etme_ncurses ${CMAKE_THREAD_LIBS_INIT})



find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM3 gtkmm-3.0 REQUIRED)
add_executable(etme_gtk gtk/gtk.cpp ${SOURCES})
target_include_directories(etme_gtk PRIVATE ${GTKMM3_INCLUDE_DIRS})
target_link_libraries(etme_gtk ${GTKMM3_LIBRARIES})
target_link_libraries(etme_gtk ${CMAKE_THREAD_LIBS_INIT})
