
build-dir:
	mkdir -p build
	cmake -B build -G Ninja

build: build-dir
	ninja -C build