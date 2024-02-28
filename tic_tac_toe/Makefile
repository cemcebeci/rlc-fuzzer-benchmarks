RLC=/home/ccebeci/rlc/rlc-infrastructure/rlc-debug/install/bin/rlc
OPEN_SPIEL_PATH=/home/ccebeci/open_spiel
BUG_DEPTH?=1

all: build/rlc_tic_tac_toe build/rlc_tic_tac_toe_no_precons build/rlc_tic_tac_toe_no_fsm build/rlc_tic_tac_toe_no_improvements build/open_spiel_whitebox_tic_tac_toe build/open_spiel_blackbox_tic_tac_toe

build/rlc_tic_tac_toe: build/tic_tac_toe.rl
	$(RLC) build/tic_tac_toe.rl -o build/rlc_tic_tac_toe --fuzzer --print-ir-on-failure=0 -O2

build/rlc_tic_tac_toe_no_precons: build/tic_tac_toe.rl
	$(RLC) build/tic_tac_toe.rl --fuzzer -o build/rlc_tic_tac_toe_no_precons --print-ir-on-failure=0 --fuzzer-analyse-preconditions=0 -O2

build/rlc_tic_tac_toe_no_fsm: build/tic_tac_toe.rl
	$(RLC) build/tic_tac_toe.rl --fuzzer -o build/rlc_tic_tac_toe_no_fsm --print-ir-on-failure=0 --fuzzer-avoid-unavailable-subactions=0 -O2

build/rlc_tic_tac_toe_no_improvements: build/tic_tac_toe.rl
	$(RLC) build/tic_tac_toe.rl --fuzzer -o build/rlc_tic_tac_toe_no_improvements --print-ir-on-failure=0 --fuzzer-avoid-unavailable-subactions=0 --fuzzer-analyse-preconditions=0 -O2

build/tic_tac_toe.rl: tic_tac_toe.rl.in
	sed "s/__NUM_GAMES__/$(value BUG_DEPTH)/g" tic_tac_toe.rl.in > build/tic_tac_toe.rl

build/open_spiel_whitebox_tic_tac_toe: ../open_spiel_whitebox.cc buggy_tic_tac_toe.cc
	clang++ -I"$(OPEN_SPIEL_PATH)" -I"$(OPEN_SPIEL_PATH)/open_spiel/abseil-cpp" -std=c++17 \
			-L"$(OPEN_SPIEL_PATH)/build" -lopen_spiel -fsanitize=fuzzer,address \
			-o build/open_spiel_whitebox_tic_tac_toe ../open_spiel_whitebox.cc -DGAME=buggy_tic_tac_toe buggy_tic_tac_toe.cc -D__NUM_GAMES__=$(BUG_DEPTH) -O2

build/open_spiel_blackbox_tic_tac_toe: ../open_spiel_blackbox.cc buggy_tic_tac_toe.cc
	clang++ -I"$(OPEN_SPIEL_PATH)" -I"$(OPEN_SPIEL_PATH)/open_spiel/abseil-cpp" -std=c++17 \
			-L"$(OPEN_SPIEL_PATH)/build" -lopen_spiel -fsanitize=fuzzer,address \
			-o build/open_spiel_blackbox_tic_tac_toe ../open_spiel_blackbox.cc -DGAME=buggy_tic_tac_toe buggy_tic_tac_toe.cc -D__NUM_GAMES__=$(BUG_DEPTH) -O2


clean:
	rm -f build/*
