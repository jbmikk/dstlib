cmake_minimum_required (VERSION 2.8.0)
project (dstlib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include_directories("${PROJECT_SOURCE_DIR}/vendor/dbg")

add_subdirectory(dstlib)
add_subdirectory(testdstlib)

