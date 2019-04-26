#!/bin/bash

MODE=${1:-run}
PATTERN=${2:-"no-pattern"}

source runner.sh

echo ""
echo "TESTSTLIB:"
echo "---------"

run_test $MODE $PATTERN ./stack_test
run_test $MODE $PATTERN ./bmap_test
run_test $MODE $PATTERN ./bmap_char_test
run_test $MODE $PATTERN ./bmap_uint_test
run_test $MODE $PATTERN ./rtree_test
run_test $MODE $PATTERN ./arrays_test
run_test $MODE $PATTERN ./result_test
