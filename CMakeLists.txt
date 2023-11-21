cmake_minimum_required(VERSION 3.0)

project(ice-engine)

option(ICE_SERVER "Build ICEngine server" OFF)
option(ICE_CLIENT "Build ICEngine client" OFF)

find_package(Vulkan)
find_package(CUDAToolkit)

add_subdirectory(extern)