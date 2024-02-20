RLC=/home/ccebeci/rlc/rlc-infrastructure/rlc-debug/install/bin/rlc

all: build/fuzzer_blackjack

build/fuzzer_blackjack: blackjack.rl
	$(RLC) blackjack.rl --fuzzer -o build/fuzzer_blackjack --print-ir-on-failure=0

clean:
	rm -f build/*
