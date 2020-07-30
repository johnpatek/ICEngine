add_executable(
    window-client
    window_client.cpp)

target_include_directories(
    window-client
    PUBLIC 
    ${PROJ_INCLUDE}
    ${COMMON_INCLUDE})

target_link_libraries(
    window-client 
    PUBLIC
    ice-engine-sdk
    ${PTHREAD})
