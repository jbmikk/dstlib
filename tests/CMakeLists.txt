include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(test_bsearch test_bsearch.c)
add_executable(test_radixtree test_radixtree.c)
add_executable(test_arrays test_arrays.c)

target_link_libraries(test_bsearch dstlib)
target_link_libraries(test_radixtree dstlib)
target_link_libraries(test_arrays dstlib)

#file(COPY test.sh DESTINATION .)

