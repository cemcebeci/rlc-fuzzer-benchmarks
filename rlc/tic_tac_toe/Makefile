RLC=/home/ccebeci/rlc/rlc-infrastructure/rlc-debug/install/bin/rlc

all: build/fuzzer_tic_tac_toe

build/fuzzer_tic_tac_toe: tic_tac_toe.rl
	$(RLC) tic_tac_toe.rl -o build/fuzzer_tic_tac_toe --fuzzer --print-ir-on-failure=0
