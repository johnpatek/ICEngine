add_executable(
    log-client
    common.h
    common.cpp
    client.cpp)

target_include_directories(
    log-client
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    log-client 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})

add_executable(
    log-server
    common.h
    common.cpp
    server.cpp)

target_include_directories(
    log-server
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    log-server 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})