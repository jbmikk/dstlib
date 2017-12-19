#!/bin/bash

MODE=${1:-run}
PATTERN=${2:-"no-pattern"}

source runner.sh

echo ""
echo "TESTSTLIB:"
echo "---------"

run_test $MODE $PATTERN ./stack_test
run_test $MODE $PATTERN ./bmap_test
run_test $MODE $PATTERN ./radixtree_test
run_test $MODE $PATTERN ./arrays_test
