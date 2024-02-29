add_library(serial_port STATIC)
add_library(hw::sport ALIAS serial_port)

target_sources(serial_port PRIVATE serial_port_linux_api.cpp)
target_include_directories(serial_port PUBLIC
    "${PROJECT_SOURCE_DIR}/include/serial_port")

install(TARGETS serial_port
    ARCHIVE 
        DESTINATION "${PROJECT_SOURCE_DIR}/lib")
