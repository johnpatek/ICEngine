add_executable(
    client
    common.h
    common.cpp
    client.cpp)

target_include_directories(
    client
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    client 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})

add_executable(
    server
    common.h
    common.cpp
    server.cpp)

target_include_directories(
    server
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    server 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})