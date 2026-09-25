.PHONY: build test clean

build:
	cmake -B build -S .
	cmake --build build

test:
	ctest --test-dir build --output-on-failure

clean:
	rm -rf build