cmake_minimum_required(VERSION 3.9)

project(etme)

set(CMAKE_CXX_COMPILER /usr/bin/clang++-5.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wextra -pedantic -Wall -Werror -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CORE_INCLUDE_DIR core/include)
set(CORE_HEADERS core/include/Buffer.h core/include/Cursor.h core/include/Optional.h core/include/LineBoundaries.h)
set(CORE_SRCS core/src/Buffer.cpp core/src/LineBoundaries.cpp)

#find_package(LLVM REQUIRED CONFIG)

#add_definitions(${LLVM_DEFINITIONS})
add_library(etme_core ${CORE_SRCS} ${CORE_HEADERS})
target_include_directories(etme_core PRIVATE ${CORE_INCLUDE_DIR})
#target_include_directories(etme_core PRIVATE ${LLVM_INCLUDE_DIRS})
#llvm_map_components_to_libnames(llvm_libs core)
#target_link_libraries(etme_core ${llvm_libs})

set(CORE_TEST_SRCS core/test/test_main.cpp core/test/BufferTest.cpp core/test/LineBoundariesTest.cpp)

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
add_executable(etme_gtk gtk/src/gtk.cpp gtk/src/BufferView.cpp gtk/include/BufferView.h)
target_include_directories(etme_gtk PRIVATE ${GTKMM3_INCLUDE_DIRS})
target_include_directories(etme_gtk PRIVATE ${CORE_INCLUDE_DIR})
target_include_directories(etme_gtk PRIVATE gtk/include)
target_link_libraries(etme_gtk etme_core)
target_link_libraries(etme_gtk ${GTKMM3_LIBRARIES})
