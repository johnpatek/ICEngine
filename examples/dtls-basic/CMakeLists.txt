add_executable(
    dtls-client
    client.cpp)

target_include_directories(
    dtls-client
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    dtls-client 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})

add_executable(
    dtls-server
    server.cpp)

target_include_directories(
    dtls-server
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    dtls-server 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})
