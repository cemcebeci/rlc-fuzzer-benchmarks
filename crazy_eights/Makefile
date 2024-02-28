RLC=/home/ccebeci/rlc/rlc-infrastructure/rlc-debug/install/bin/rlc
OPEN_SPIEL_PATH=/home/ccebeci/open_spiel
BUG_DEPTH?=8

all: build/open_spiel_whitebox_crazy_eights build/open_spiel_blackbox_crazy_eights build/rlc_crazy_eights build/rlc_crazy_eights_no_precons build/rlc_crazy_eights_no_fsm build/rlc_crazy_eights_no_improvements

build/rlc_crazy_eights: build/crazy_eights.rl build/io_nodebug.o
	$(RLC) build/crazy_eights.rl --fuzzer -o build/rlc_crazy_eights build/io_nodebug.o --print-ir-on-failure=0 -O2

build/rlc_crazy_eights_no_precons: build/crazy_eights.rl build/io_nodebug.o
	$(RLC) build/crazy_eights.rl --fuzzer -o build/rlc_crazy_eights_no_precons build/io_nodebug.o --print-ir-on-failure=0 --fuzzer-analyse-preconditions=0 -O2

build/rlc_crazy_eights_no_fsm: build/crazy_eights.rl build/io_nodebug.o
	$(RLC) build/crazy_eights.rl --fuzzer -o build/rlc_crazy_eights_no_fsm build/io_nodebug.o --print-ir-on-failure=0 --fuzzer-avoid-unavailable-subactions=0 -O2

build/rlc_crazy_eights_no_improvements: build/crazy_eights.rl build/io_nodebug.o
	$(RLC) build/crazy_eights.rl --fuzzer -o build/rlc_crazy_eights_no_improvements build/io_nodebug.o --print-ir-on-failure=0 --fuzzer-avoid-unavailable-subactions=0 --fuzzer-analyse-preconditions=0 -O2

build/crazy_eights.rl: crazy_eights.rl.in
	sed "s/__SUIT_COUNT__/$(BUG_DEPTH)/g" crazy_eights.rl.in > build/crazy_eights.rl

build/replayer_crazy_eights: crazy_eights.rl build/io.o
	$(RLC) crazy_eights.rl --fuzzer -o build/replayer_crazy_eights build/io.o --print-ir-on-failure=0

build/io_nodebug.o: io.cpp
	clang++ io.cpp -DNODEBUGINFO -c -o build/io_nodebug.o

build/io.o: io.cpp
	clang++ io.cpp -c -o build/io.o

build/open_spiel_whitebox_crazy_eights: ../open_spiel_whitebox.cc buggy_crazy_eights.cc
	clang++ -I"$(OPEN_SPIEL_PATH)" -I"$(OPEN_SPIEL_PATH)/open_spiel/abseil-cpp" -std=c++17 \
			-L"$(OPEN_SPIEL_PATH)/build" -lopen_spiel -fsanitize=fuzzer,address \
			-o build/open_spiel_whitebox_crazy_eights ../open_spiel_whitebox.cc -DGAME="buggy_crazy_eights(reshuffle=true,use_special_cards=false,players=2,max_draw_cards=3)" buggy_crazy_eights.cc -D__NUM_RANKS__=$(BUG_DEPTH) -O2

build/open_spiel_blackbox_crazy_eights: ../open_spiel_blackbox.cc buggy_crazy_eights.cc
	clang++ -g -I"$(OPEN_SPIEL_PATH)" -I"$(OPEN_SPIEL_PATH)/open_spiel/abseil-cpp" -std=c++17 \
			-L"$(OPEN_SPIEL_PATH)/build" -lopen_spiel -fsanitize=fuzzer,address \
			-o build/open_spiel_blackbox_crazy_eights ../open_spiel_blackbox.cc -DGAME="buggy_crazy_eights(reshuffle=true,use_special_cards=false,players=2,max_draw_cards=3)" buggy_crazy_eights.cc -D__NUM_RANKS__=$(BUG_DEPTH) -O2

clean:
	rm -f build/*