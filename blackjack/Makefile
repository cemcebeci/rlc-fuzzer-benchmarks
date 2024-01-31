RLC=/home/ccebeci/rlc/rlc-infrastructure/rlc-debug/install/bin/rlc

all: build/fuzzer_blackjack

build/fuzzer_blackjack: blackjack.rl build/blackjack.o build/class_to_global_functions.o
	$(RLC) blackjack.rl -o build/fuzzer_blackjack --fuzzer build/blackjack.o build/class_to_global_functions.o

build/blackjack.o: blackjack.cpp
	clang++ -c blackjack.cpp -o build/blackjack.o

build/class_to_global_functions.o: class_to_global_functions.cpp build/rlc_names.h
	clang++ -c class_to_global_functions.cpp -o build/class_to_global_functions.o

build/rlc_names.h:
	$(RLC) blackjack.rl -o build/rlc_names.h --header

clean:
	rm -f build/*
