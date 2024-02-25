RLC=/home/ccebeci/rlc/rlc-infrastructure/rlc-debug/install/bin/rlc
OPEN_SPIEL_PATH=/home/ccebeci/open_spiel
BUG_DEPTH?=1

all: build/rlc_blackjack build/rlc_blackjack_no_precons build/rlc_blackjack_no_fsm build/rlc_blackjack_no_improvements build/open_spiel_whitebox_blackjack build/open_spiel_blackbox_blackjack

build/rlc_blackjack: build/blackjack.rl
	$(RLC) build/blackjack.rl --fuzzer -o build/rlc_blackjack --print-ir-on-failure=0

build/rlc_blackjack_no_precons: build/blackjack.rl
	$(RLC) build/blackjack.rl --fuzzer -o build/rlc_blackjack_no_precons --print-ir-on-failure=0 --fuzzer-analyse-preconditions=0

build/rlc_blackjack_no_fsm: build/blackjack.rl
	$(RLC) build/blackjack.rl --fuzzer -o build/rlc_blackjack_no_fsm --print-ir-on-failure=0 --fuzzer-avoid-unavailable-subactions=0

build/rlc_blackjack_no_improvements: build/blackjack.rl
	$(RLC) build/blackjack.rl --fuzzer -o build/rlc_blackjack_no_improvements --print-ir-on-failure=0 --fuzzer-avoid-unavailable-subactions=0 --fuzzer-analyse-preconditions=0

build/blackjack.rl: blackjack.rl.in
	sed "s/__TARGET_SCORE_MULTIPLIER__/$(BUG_DEPTH)/g" blackjack.rl.in > build/blackjack.rl

build/open_spiel_whitebox_blackjack: ../open_spiel_whitebox.cc buggy_blackjack.cc
	clang++ -I"$(OPEN_SPIEL_PATH)" -I"$(OPEN_SPIEL_PATH)/open_spiel/abseil-cpp" -std=c++17 \
			-L"$(OPEN_SPIEL_PATH)/build" -lopen_spiel -fsanitize=fuzzer,address \
			-o build/open_spiel_whitebox_blackjack ../open_spiel_whitebox.cc -DGAME=buggy_blackjack buggy_blackjack.cc -D__TARGET_SCORE_MULTIPLIER__=$(BUG_DEPTH)

build/open_spiel_blackbox_blackjack: ../open_spiel_blackbox.cc buggy_blackjack.cc
	clang++ -I"$(OPEN_SPIEL_PATH)" -I"$(OPEN_SPIEL_PATH)/open_spiel/abseil-cpp" -std=c++17 \
			-L"$(OPEN_SPIEL_PATH)/build" -lopen_spiel -fsanitize=fuzzer,address \
			-o build/open_spiel_blackbox_blackjack ../open_spiel_blackbox.cc -DGAME=buggy_blackjack buggy_blackjack.cc -D__TARGET_SCORE_MULTIPLIER__=$(BUG_DEPTH)

clean:
	rm -f build/*
