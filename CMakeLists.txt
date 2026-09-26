cmake_minimum_required (VERSION 3.20.0)
project (dstlib LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON) # Only needed for typeof

include(FetchContent)
include(CTest)

option(PROFILE "Enable profiling options" OFF)


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


# Include externals
# -----------------

FetchContent_Declare(
        dbg
        GIT_REPOSITORY https://github.com/jbmikk/dbg.git
        GIT_TAG 1.0.1
)

FetchContent_Declare(
        ctest
        GIT_REPOSITORY https://github.com/jbmikk/ctest.git
        GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(dbg ctest)


# Dstlib
# ------

file(GLOB SOURCES src/*.c)

add_library(dstlib ${SOURCES})

target_include_directories(dstlib PRIVATE
        "${dbg_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/src"
)

target_link_libraries(dstlib PRIVATE dbg ctest)

target_compile_options(dstlib PRIVATE -Wall)

target_compile_definitions(dstlib PRIVATE LIBRARY)

if(ENABLE_RTREE_TRACE)
        target_compile_definitions(dstlib PRIVATE RTREE_TRACE)
endif()


# Tests
# -----

set(TEST_TARGETS
        stack_test
        bmap_test
        bmap_char_test
        bmap_uint_test
        rtree_test
        arrays_test
        result_test
)

find_program(VALGRIND_EXECUTABLE valgrind)
find_program(GPROF_EXECUTABLE gprof)

foreach(test_target IN LISTS TEST_TARGETS)
        add_executable(${test_target} test/${test_target}.c)

        target_include_directories(${test_target} PRIVATE
                "${PROJECT_SOURCE_DIR}/src"
                "${ctest_SOURCE_DIR}/src"
        )
        target_compile_options(${test_target} PRIVATE -Wall)
        target_link_libraries(${test_target} PRIVATE dstlib)

        add_test(NAME ${test_target} COMMAND ${test_target})
        set_tests_properties(${test_target} PROPERTIES LABELS regular)

        if(GPROF_EXECUTABLE)
                # TODO: Save output "gprof executable gmon.out > output.txt"
                add_test(NAME ${test_target}_profile COMMAND "${GPROF_EXECUTABLE}" "$<TARGET_FILE:${test_target}>")
                set_tests_properties(${test_target}_profile PROPERTIES LABELS profile)
        endif()

        if(VALGRIND_EXECUTABLE)
                add_test(NAME ${test_target}_valgrind
                        COMMAND "${VALGRIND_EXECUTABLE}" --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 --suppressions=${PROJECT_SOURCE_DIR}/suppressions.supp "$<TARGET_FILE:${test_target}>"
                )
                set_tests_properties(${test_target}_valgrind PROPERTIES LABELS valgrind)

                add_test(NAME ${test_target}_callgrind_calls
                        COMMAND "${VALGRIND_EXECUTABLE}" --tool=callgrind "$<TARGET_FILE:${test_target}>"
                )
                set_tests_properties(${test_target}_callgrind_calls PROPERTIES LABELS callgrind_calls)

                add_test(NAME ${test_target}_callgrind_simulate_cache
                        COMMAND "${VALGRIND_EXECUTABLE}" --tool=callgrind --simulate-cache=yes "$<TARGET_FILE:${test_target}>"
                )
                set_tests_properties(${test_target}_callgrind_simulate_cache PROPERTIES LABELS callgrind_simulate_cache)
        endif()
endforeach()
