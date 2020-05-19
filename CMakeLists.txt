cmake_minimum_required(VERSION 3.0)

project(ice-engine C CXX)

set(OPENSSL_USE_STATIC_LIBS TRUE)

set(COMMON_ROOT ${CMAKE_SOURCE_DIR}/common/build/install)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

set(COMMON_INCLUDE ${COMMON_ROOT}/include)

set(COMMON_LIB ${COMMON_ROOT}/lib)

set(PROJ_INCLUDE ${CMAKE_SOURCE_DIR}/include)

include_directories(${PROJ_INCLUDE} ${COMMON_INCLUDE})

if(WIN32)
    set(PTHREAD "")
    find_library(SSL_LIB_NAME libssl-1_1-x64.lib PATHS ${COMMON_LIB})
    find_library(SDL2_LIB_NAME SDL2_staticd.lib PATHS ${COMMON_LIB})
    find_library(CRYPTO_LIB_NAME libcrypto-1_1-x64.lib PATHS ${COMMON_LIB})
    set(DL "")
else()
    set(CMAKE_CXX_FLAGS  "-std=c++14")
    set(PTHREAD pthread)
    find_library(SSL_LIB_NAME libssl.a PATHS ${COMMON_LIB})
    find_library(CRYPTO_LIB_NAME libcrypto.a PATHS ${COMMON_LIB})
    find_library(SDL2_LIB_NAME SDL2_staticd.a PATHS ${COMMON_LIB})
    find_library(DL libdl.so PATHS /usr/lib/x86_64-linux-gnu/)
endif(WIN32)

file(GLOB ENGINE_SOURCES
    "include/ice-engine/*.h"    
    "src/ice-engine/*.cpp")

if(WIN32)
    add_library(
        ice-engine-sdk 
        STATIC
        ${ENGINE_SOURCES})
else()
    add_library(
        ice-engine-sdk 
        SHARED
        ${ENGINE_SOURCES})
endif()

target_include_directories(
    ice-engine-sdk 
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    ice-engine-sdk 
    PUBLIC
    ${DL}
    ${PTHREAD}
    ${SSL_LIB_NAME}
    ${CRYPTO_LIB_NAME}
    ${SDL2_LIB_NAME})

file(GLOB CLIENT_SOURCES
    "include/ice-engine/*.h"
    "test/ice-engine/client.cpp")

add_executable(
    client
    ${CLIENT_SOURCES})

target_include_directories(
    client
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    client 
    PUBLIC
    ice-engine-sdk)

file(GLOB SERVER_SOURCES
    "include/ice-engine/*.h"
    "test/ice-engine/server.cpp")

add_executable(
    server
    ${SERVER_SOURCES})

target_include_directories(
    server
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    server 
    PUBLIC
    ice-engine-sdk)