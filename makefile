
.PHONY: src build examples include test

all: run

.ONESHELL:
vcpkg:
	git clone https://github.com/Microsoft/vcpkg.git --depth=1
	bash ./vcpkg/bootstrap-vcpkg.sh

install_deps:
	./vcpkg/vcpkg install libfmt




setup:
	cmake -S . -B build

compile: setup
	$(MAKE) -s -C "build"

run: compile setup
	"./build/Kaleidoscope" $(args)
	
test: compile
	"./build/TestRunner"

count:
	cloc src/ include/




