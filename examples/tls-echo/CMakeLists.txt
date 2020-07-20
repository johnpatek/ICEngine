add_executable(
    echo-client
    client.cpp
    echo.h
    echo.cpp)

target_include_directories(
    echo-client
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    echo-client 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})

add_executable(
    echo-server
    server.cpp
    echo.h
    echo.cpp)

target_include_directories(
    echo-server
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    echo-server 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})