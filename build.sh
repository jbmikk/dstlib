mkdir -p build
cd build

echo "BUILD PROJECT"
TRACES=${1:-OFF}

echo TRACES = $TRACES

echo ""
echo "CMAKE:"
echo "------"
cmake ..

echo ""
echo "MAKE:"
echo "-----"
make

cd ..
