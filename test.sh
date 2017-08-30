#!/bin/bash

MODE=${1:-run}
PATTERN=${2:-"no-pattern"}

source runner.sh

echo ""
echo "TESTSTLIB:"
echo "---------"

run_test $MODE $PATTERN ./test_stack
run_test $MODE $PATTERN ./test_bsearch
run_test $MODE $PATTERN ./test_radixtree
run_test $MODE $PATTERN ./test_arrays
