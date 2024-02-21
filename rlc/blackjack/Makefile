RLC=/home/ccebeci/rlc/rlc-infrastructure/rlc-debug/install/bin/rlc
BUG_DEPTH?=1

all: build/fuzzer_blackjack

build/fuzzer_blackjack: build/blackjack.rl
	$(RLC) build/blackjack.rl --fuzzer -o build/fuzzer_blackjack --print-ir-on-failure=0

build/blackjack.rl: blackjack.rl.in
	sed "s/__TARGET_SCORE_MULTIPLIER__/$(BUG_DEPTH)/g" blackjack.rl.in > build/blackjack.rl
clean:
	rm -f build/*
