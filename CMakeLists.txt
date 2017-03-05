cmake_minimum_required (VERSION 2.8.0)
project (dstlib)

include(ExternalProject)

option(PROFILE "Enable profiling options" OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")


# Traces
# ------

option(ENABLE_RADIXTREE_TRACE "Enable radixtree traces" OFF)
option(TRACE "Enable all traces" OFF)

if(TRACE)
        set(ENABLE_RADIXTREE_TRACE ON)
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

if(ENABLE_RADIXTREE_TRACE)
        set(RADIXTREE_TRACE_FLAGS "-DRADIXTREE_TRACE")
endif(ENABLE_RADIXTREE_TRACE)

set(DSTLIB_FLAGS "${RADIXTREE_TRACE_FLAGS}")


# Dstlib
# ------

file(GLOB SOURCES src/*.c)

add_library(dstlib ${SOURCES})

add_dependencies(dstlib dbg)

set_target_properties(dstlib PROPERTIES COMPILE_FLAGS "-DLIBRARY ${DSTLIB_FLAGS}")


# Tests
# -----

include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(test_bsearch tests/test_bsearch.c)
add_executable(test_radixtree tests/test_radixtree.c)
add_executable(test_arrays tests/test_arrays.c)

target_link_libraries(test_bsearch dstlib)
target_link_libraries(test_radixtree dstlib)
target_link_libraries(test_arrays dstlib)
