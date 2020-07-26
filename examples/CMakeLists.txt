add_subdirectory(tls-echo)
add_subdirectory(dtls-basic)
add_subdirectory(tls-logger)

if(WIN32)
    add_subdirectory(gui-basic)
endif()