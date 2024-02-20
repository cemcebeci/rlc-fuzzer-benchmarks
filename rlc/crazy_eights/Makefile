RLC=/home/ccebeci/rlc/rlc-infrastructure/rlc-debug/install/bin/rlc

all: build/fuzzer_crazy_eights build/replayer_crazy_eights

build/fuzzer_crazy_eights: crazy_eights.rl build/io_nodebug.o
	$(RLC) crazy_eights.rl --fuzzer -o build/fuzzer_crazy_eights build/io_nodebug.o

build/replayer_crazy_eights: crazy_eights.rl build/io.o
	$(RLC) crazy_eights.rl --fuzzer -o build/replayer_crazy_eights build/io.o

build/io_nodebug.o: io.cpp
	clang++ io.cpp -DNODEBUGINFO -c -o build/io_nodebug.o

build/io.o: io.cpp
	clang++ io.cpp -c -o build/io.o


clean:
	rm -f build/*
