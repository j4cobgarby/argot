cmake_minimum_required(VERSION 3.1)
project(Argot)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

find_package(PkgConfig)
find_package(Threads)
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(argot
    main.cpp
    cmdarg.cpp
    network/server.cpp
    network/handle_client_msg.cpp
    network/handle_select.cpp
    gui/resources.c)

target_link_libraries(argot
    ${GTKMM_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT})
