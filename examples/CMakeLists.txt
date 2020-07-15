add_executable(
    tcp-client
    tcp_client.cpp)

target_include_directories(
    tcp-client
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    tcp-client 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})

add_executable(
    tcp-server
    tcp_server.cpp)

target_include_directories(
    tcp-server
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    tcp-server 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})

add_executable(
    udp-client
    udp_client.cpp)

target_include_directories(
    udp-client
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    udp-client 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})

add_executable(
    udp-server
    udp_server.cpp)

target_include_directories(
    udp-server
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    udp-server 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})