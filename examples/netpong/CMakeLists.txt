add_executable(
    netpong-client
    netpong.cpp
    client.cpp)

target_include_directories(
    netpong-client
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    netpong-client 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})

add_executable(
    netpong-server
    netpong.cpp
    server.cpp)

target_include_directories(
    netpong-server
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    netpong-server 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})
