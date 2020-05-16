cmake_minimum_required(VERSION 3.0)

project(ice-engine)

#set(OPENSSL_USE_STATIC_LIBS TRUE)

set(COMMON_ROOT ${CMAKE_SOURCE_DIR}/common/build/install)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

set(COMMON_INCLUDE ${COMMON_ROOT}/include)

set(COMMON_LIB ${COMMON_ROOT}/lib)

set(PROJ_INCLUDE ${CMAKE_SOURCE_DIR}/include)

include_directories(${PROJ_INCLUDE} ${COMMON_INCLUDE})

if(WIN32)
    set(PTHREAD "")
    find_library(SSL_LIB_NAME libssl-1_1-x64.lib PATHS ${COMMON_LIB})
    find_library(CRYPTO_LIB_NAME libcrypto-1_1-x64.lib PATHS ${COMMON_LIB})
else()
    set(CMAKE_CXX_FLAGS  "-std=c++14")
    set(PTHREAD pthread)
    find_library(SSL_LIB_NAME libssl.a PATHS ${COMMON_LIB})
    find_library(CRYPTO_LIB_NAME libcrypto.a PATHS ${COMMON_LIB})
endif(WIN32)

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
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    ice-engine-sdk 
    PUBLIC
    #SDL2_staticd
    ${PTHREAD}
    ${SSL_LIB_NAME}
    ${CRYPTO_LIB_NAME})