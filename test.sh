#setup $PWD variable
cd -P .

ROOT=$PWD

run_test()
{
	VALOPTIONS="--suppressions=$ROOT/suppressions.supp --leak-check=full --show-leak-kinds=all --track-origins=yes"

	# test if command matches pattern
	if [ "$2" = "no-pattern" ]; then
		MATCHES=0
	else
		echo "$3"|grep "$2">/dev/null;
		MATCHES=$?
	fi

	# if they match then run command
	if [ $MATCHES -eq 0 ]; then

		if [ "$1" = "leaks" ]; then
			valgrind $VALOPTIONS $3
		fi

		if [ "$1" = "calls" ]; then
			echo $3
			OUTPUT=$(valgrind --tool=callgrind $3)
			echo "Usage: callgrind_annotate --auto=yes callgrind.out.pid"
		fi

		if [ "$1" = "cache" ]; then
			echo $3
			OUTPUT=$(valgrind --tool=callgrind --simulate-cache=yes $3)
			echo "Usage: callgrind_annotate --auto=yes callgrind.out.pid"
		fi

		if [ "$1" = "run" -o "$1" = "trace" ]; then
			$3
		fi

	fi
}

MODE=${1:-run}
echo "MODE: $MODE"

if [ "$MODE" = "trace" ]; then
	TRACE="ON"
else
	TRACE="OFF"
fi

if [ "$MODE" = "prof" ]; then
	PROFILE="ON"
else
	PROFILE="OFF"
fi

PATTERN=${2:-"no-pattern"}


# Build
mkdir -p build
cd build

cmake .. -DTRACE=$TRACE -DPROFILE=$PROFILE
make


echo ""
echo "TESTSTLIB:"
echo "---------"

run_test $MODE $PATTERN ./test_bsearch
run_test $MODE $PATTERN ./test_radixtree
run_test $MODE $PATTERN ./test_arrays

cd ..

cd ..

# TODO: short version for leaks
# TODO: add colors for output, specially test results
# TODO: save output?
# >> output.txt 2>&1
