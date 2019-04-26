
DSTLIB: Data structure library
==============================

A data structure library for C

It contains:
* A radix tree
* A simple stack


Build
-----

In order to build this library you need:
* cmake


Test
----

The tests give good insights into how to use the library.


Debugging
---------

This project uses lots of macros, in order to debug files it's often useful to
preprocess files. CMake generates preprocessor targets for each file. So you
can generate preprocessed files like this:

    make src/bmap.i

License
-------

Licensed under the [GPL version 2] (http://www.gnu.org/licenses/)

Copyright 2016 Jens Mikkelsen.



