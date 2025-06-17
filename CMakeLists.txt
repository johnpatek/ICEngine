cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ice-engine VERSION 1.0.0)

option(ICE_SERVER "Build ICEngine server" ON)
option(ICE_CLIENT "Build ICEngine client" ON)
option(ICE_TESTS "Build ICEngine tests" ON)

include(FetchContent)

find_package(flatbuffers CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(vsg CONFIG REQUIRED)

if(ICE_TESTS)
    include(CTest)
    enable_testing()
    FetchContent_Declare(
        Maxtest
        GIT_REPOSITORY https://github.com/maxtek6/maxtest.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(Maxtest)
endif()

add_subdirectory(nsg)