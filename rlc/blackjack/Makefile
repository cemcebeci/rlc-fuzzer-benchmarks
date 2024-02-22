RLC=/home/ccebeci/rlc/rlc-infrastructure/rlc-debug/install/bin/rlc
BUG_DEPTH?=1

all: build/fuzzer_blackjack build/fuzzer_blackjack_no_precons build/fuzzer_blackjack_no_fsm build/fuzzer_blackjack_no_improvements

build/fuzzer_blackjack: build/blackjack.rl
	$(RLC) build/blackjack.rl --fuzzer -o build/fuzzer_blackjack --print-ir-on-failure=0

build/fuzzer_blackjack_no_precons: build/blackjack.rl
	$(RLC) build/blackjack.rl --fuzzer -o build/fuzzer_blackjack_no_precons --print-ir-on-failure=0 --fuzzer-analyse-preconditions=0

build/fuzzer_blackjack_no_fsm: build/blackjack.rl
	$(RLC) build/blackjack.rl --fuzzer -o build/fuzzer_blackjack_no_fsm --print-ir-on-failure=0 --fuzzer-avoid-unavailable-subactions=0

build/fuzzer_blackjack_no_improvements: build/blackjack.rl
	$(RLC) build/blackjack.rl --fuzzer -o build/fuzzer_blackjack_no_improvements --print-ir-on-failure=0 --fuzzer-avoid-unavailable-subactions=0 --fuzzer-analyse-preconditions=0

build/blackjack.rl: blackjack.rl.in
	sed "s/__TARGET_SCORE_MULTIPLIER__/$(BUG_DEPTH)/g" blackjack.rl.in > build/blackjack.rl
clean:
	rm -f build/*
