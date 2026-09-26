.PHONY: build test build-run build-trace build-profile test-run test-trace test-profile test-leak test-leaktrace test-calls test-cache clean

build:
	cmake -B build -S .
	cmake --build build

test:
	ctest --test-dir build --output-on-failure

build-run:
	cmake --preset run
	cmake --build --preset run

build-trace:
	cmake --preset trace
	cmake --build --preset trace

build-profile:
	cmake --preset profile
	cmake --build --preset profile

test-run: build-run
	ctest --preset run -L regular $(if $(R),-R "$(R)")

test-trace: build-trace
	ctest --preset trace -L regular $(if $(R),-R "$(R)")

test-profile-run: build-profile
	ctest --preset profile -L regular $(if $(R),-R "$(R)")

test-profile: test-profile-run
	ctest --preset profile -L profile $(if $(R),-R "$(R)")

test-leak: build-run
	ctest --preset run -L valgrind $(if $(R),-R "$(R)")

test-leaktrace: build-trace
	ctest --preset trace -L valgrind $(if $(R),-R "$(R)")

test-calls: build-run
	ctest --preset run -L callgrind_calls $(if $(R),-R "$(R)")

test-cache: build-run
	ctest --preset run -L callgrind_simulate_cache $(if $(R),-R "$(R)")

clean:
	rm -rf build