cmake_minimum_required(VERSION 2.8)
project(memwatch)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE FALSE)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

include_directories(.)
add_executable(memwatch
               main.c
               options.c
               common.c
               mem_fields.c
               memory_info.c
               process_info.c
               vector.c)
target_link_libraries(${PROJECT_NAME}
                    ${CURSES_LIBRARIES}
                    m)

install(TARGETS memwatch DESTINATION bin)
