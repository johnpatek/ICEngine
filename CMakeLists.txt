cmake_minimum_required(VERSION 3.0)

project(ice-engine)

set(CXX_STANDARD 14)

include(CTest)

set(3RDPARTY ../ice-engine-3rdparty/install)
set(CORE ../ice-engine-core/install)

set(3RDPARTY_INCLUDE ${3RDPARTY}/include)
set(CORE_INCLUDE ${CORE}/include)

set(3RDPARTY_LIB ${3RDPARTY}/lib)
set(CORE_LIB ${CORE}/lib)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)

include_directories(include ${3RDPARTY_INCLUDE} ${CORE_INCLUDE})
link_directories(${3RDPARTY_LIB} ${CORE_LIB})

add_executable(client client.cpp)

target_link_libraries(client ice-engine-core uWS uv z pthread ssl)

target_include_directories(client PUBLIC ${3RDPARTY_INCLUDE})
target_include_directories(client PUBLIC ${CORE_INCLUDE})

add_executable(server server.cpp)

target_link_libraries(server ice-engine-core uWS uv z pthread ssl)

target_include_directories(server PUBLIC ${3RDPARTY_INCLUDE})
target_include_directories(server PUBLIC ${CORE_INCLUDE})