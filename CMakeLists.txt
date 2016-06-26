cmake_minimum_required (VERSION 2.8.0)
project (dstlib)


# Dependencies

include_directories("${PROJECT_SOURCE_DIR}/vendor/dbg")

add_custom_target(
        dependencies ALL sh install.sh
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)


# Dstlib

file(GLOB SOURCES src/*.c)

add_library(dstlib ${SOURCES})


# Tests

include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(test_bsearch tests/test_bsearch.c)
add_executable(test_radixtree tests/test_radixtree.c)
add_executable(test_arrays tests/test_arrays.c)

target_link_libraries(test_bsearch dstlib)
target_link_libraries(test_radixtree dstlib)
target_link_libraries(test_arrays dstlib)
