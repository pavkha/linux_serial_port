cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(cxx-common-opts)

project(linux_serial_port VERSION 1.0)

find_package(GTest REQUIRED)

#For autorun ctest after build add -D AUTORUN_CTEST=ON in cmake -B build command
option(AUTORUN_CTEST OFF)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
