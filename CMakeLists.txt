cmake_minimum_required (VERSION 2.8.0)
project (dstlib)

include(ExternalProject)

option(PROFILE "Enable profiling options" OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")


# Traces
# ------

option(ENABLE_RTREE_TRACE "Enable rtree traces" OFF)
option(TRACE "Enable all traces" OFF)

if(TRACE)
        set(ENABLE_RTREE_TRACE ON)
endif(TRACE)

# Profiling
# ---------

if(PROFILE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
endif(PROFILE)


# Include external dbg
# --------------------

include_directories("${CMAKE_BINARY_DIR}/dbg-prefix/src/dbg")

externalproject_add(
        dbg
        GIT_REPOSITORY git@github.com:jbmikk/dbg.git
        DOWNLOAD_NAME dbg
        DOWNLOAD_DIR dbg
        INSTALL_COMMAND ""
        UPDATE_DISCONNECTED 1
)


# Flags
# -----

if(ENABLE_RTREE_TRACE)
        set(RTREE_TRACE_FLAGS "-DRTREE_TRACE")
endif(ENABLE_RTREE_TRACE)

set(DSTLIB_FLAGS "${RTREE_TRACE_FLAGS}")


# Dstlib
# ------

file(GLOB SOURCES src/*.c)

add_library(dstlib ${SOURCES})

add_dependencies(dstlib dbg)

set_target_properties(dstlib PROPERTIES COMPILE_FLAGS "-DLIBRARY ${DSTLIB_FLAGS}")


# Tests
# -----

include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(stack_test test/stack_test.c)
add_executable(bmap_test test/bmap_test.c)
add_executable(bmap_char_test test/bmap_char_test.c)
add_executable(bmap_uint_test test/bmap_uint_test.c)
add_executable(rtree_test test/rtree_test.c)
add_executable(arrays_test test/arrays_test.c)
add_executable(result_test test/result_test.c)

target_link_libraries(stack_test dstlib)
target_link_libraries(bmap_test dstlib)
target_link_libraries(bmap_char_test dstlib)
target_link_libraries(bmap_uint_test dstlib)
target_link_libraries(rtree_test dstlib)
target_link_libraries(arrays_test dstlib)
target_link_libraries(result_test dstlib)
