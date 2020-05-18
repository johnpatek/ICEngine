cmake_minimum_required(VERSION 3.0)

project(ice-common)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

add_subdirectory(sdl2)
add_subdirectory(openssl-cmake)