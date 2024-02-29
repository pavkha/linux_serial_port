add_executable(serial_port_linux_api_test
        serial_port_linux_api_test.cpp)

target_link_libraries(serial_port_linux_api_test
        hw::sport
        GTest::Main)

include(GoogleTest)
gtest_discover_tests(serial_port_linux_api_test)

if(AUTORUN_CTEST)
add_custom_command(
        TARGET serial_port_linux_api_test POST_BUILD
        COMMAND ctest -V
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
