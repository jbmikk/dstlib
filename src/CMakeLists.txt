add_library (
        dstlib
        arrays.c
        stack.c
        radixtree.c
        cmemory.c
        bsearch.c
)

export(TARGETS dstlib FILE dstlib-exports.cmake)
