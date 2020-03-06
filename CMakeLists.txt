cmake_minimum_required(VERSION 3.0)

project(ice-engine)

SET(CMAKE_CXX_FLAGS  "-std=c++14")

set(3RDPARTY ../ice-engine-3rdparty/build/install)

set(3RDPARTY_INCLUDE ${3RDPARTY}/include)

set(3RDPARTY_LIB ${3RDPARTY}/lib)

set(CORE ../ice-engine-core/build/install)

set(CORE_INCLUDE ${CORE}/include)

set(CORE_LIB ${CORE}/lib)

find_library(ENGINE_CORE NAMES ice-engine-core HINTS ${CORE_LIB})

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)

include_directories(include ${3RDPARTY_INCLUDE} ${CORE_INCLUDE})
link_directories(${3RDPARTY_LIB} ${CORE_LIB})

add_executable(server server.cpp)

target_include_directories(server PUBLIC ${3RDPARTY_INCLUDE})
target_include_directories(server PUBLIC ${CORE_INCLUDE})
target_include_directories(server PUBLIC ${ZLIB_INCLUDE_DIRS})

if(WIN32)
    set(THREADS "")
else()
    set(THREADS pthread)
endif()

target_link_libraries (
    server
    ${ENGINE_CORE}
    ${THREADS}
    uWS
    ssl
    crypto)

add_executable(client client.cpp)

target_include_directories(client PUBLIC ${3RDPARTY_INCLUDE})
target_include_directories(client PUBLIC ${CORE_INCLUDE})
target_include_directories(client PUBLIC ${ZLIB_INCLUDE_DIRS})

target_link_libraries (
    client
    ${ENGINE_CORE}
    ${THREADS}
    ssl
    crypto
    curl)
