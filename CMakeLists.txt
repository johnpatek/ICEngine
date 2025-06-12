cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(ice-engine VERSION 1.0.0)

option(ICE_SERVER "Build ICEngine server" ON)
option(ICE_CLIENT "Build ICEngine client" ON)

