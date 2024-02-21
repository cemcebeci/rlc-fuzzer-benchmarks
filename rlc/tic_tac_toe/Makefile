RLC=/home/ccebeci/rlc/rlc-infrastructure/rlc-debug/install/bin/rlc
BUG_DEPTH?=3

all: build/fuzzer_tic_tac_toe

build/fuzzer_tic_tac_toe: build/tic_tac_toe.rl
	$(RLC) build/tic_tac_toe.rl -o build/fuzzer_tic_tac_toe --fuzzer --print-ir-on-failure=0

build/tic_tac_toe.rl: tic_tac_toe.rl.in
	sed "s/__BOARD_SIZE__/$(value BUG_DEPTH)/g; s/__BOARD_SIZE_SQUARED__/$(shell expr $(BUG_DEPTH) \* $(BUG_DEPTH))/g" tic_tac_toe.rl.in > build/tic_tac_toe.rl

clean:
	rm -f build/*
