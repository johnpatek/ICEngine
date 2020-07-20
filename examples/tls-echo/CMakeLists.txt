add_executable(
    tls-client
    client.cpp
    echo.h
    echo.cpp)

target_include_directories(
    tls-client
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    tls-client 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})

add_executable(
    tls-server
    server.cpp
    echo.h
    echo.cpp)

target_include_directories(
    tls-server
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    tls-server 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})