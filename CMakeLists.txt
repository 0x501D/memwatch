cmake_minimum_required(VERSION 2.8)
project(memwatch C)

set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic")
add_subdirectory(src)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# install man page  
INSTALL(FILES memwatch.1 DESTINATION share/man/man1)
