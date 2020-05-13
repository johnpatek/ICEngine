cmake_minimum_required(VERSION 3.0)

project(ice-engine)

set(CMAKE_CXX_FLAGS  "-std=c++14")

if(WIN32)
    set(PTHREAD "")
    set(SSL_LIB_NAME libssl-1_1-x64.lib)
    set(CRYPTO_LIB_NAME libcrypto-1_1-x64.lib)
else()
    set(PTHREAD pthread)
    set(SSL_LIB_NAME ssl)
    set(CRYPTO_LIB_NAME crypto)
endif(WIN32)

set(OPENSSL_USE_STATIC_LIBS TRUE)

set(COMMON_ROOT ${CMAKE_SOURCE_DIR}/common/build/install)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

set(COMMON_INCLUDE ${COMMON_ROOT}/include)

set(COMMON_LIB ${COMMON_ROOT}/lib)

include_directories(include ${COMMON_INCLUDE})

file(GLOB ENGINE_SOURCES
    "include/ice-engine/*.h"    
    "src/ice-engine/*.cpp")

add_library(
    ice-engine-sdk 
    SHARED 
    ${ENGINE_SOURCES})

target_include_directories(
    ice-engine-sdk 
    PUBLIC 
    include 
    ${COMMON_INCLUDE})

target_link_libraries(
    ice-engine-sdk 
    PUBLIC
    SDL2_static
    ${PTHREAD}
    ${SSL_LIB_NAME}
    ${CRYPTO_LIB_NAME})

file(GLOB ENGINE_SOURCES
    "include/ice-engine/*.h"
    "test/*.cpp")

add_executable(unittest)