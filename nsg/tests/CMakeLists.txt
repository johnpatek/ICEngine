maxtest_add_executable(nsgtest
  ${CMAKE_CURRENT_SOURCE_DIR}/unit.cpp
)

target_link_libraries(nsgtest
  PRIVATE
    nsg
)

maxtest_add_test(nsgtest command_test)
maxtest_add_test(nsgtest reader_test)