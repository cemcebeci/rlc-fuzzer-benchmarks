RLC=/home/ccebeci/rlc/rlc-infrastructure/rlc-debug/install/bin/rlc
OPEN_SPIEL_PATH=/home/ccebeci/open_spiel
BUG_DEPTH?=3

all: build/open_spiel_whitebox_crazy_eights build/open_spiel_blackbox_crazy_eights

build/open_spiel_whitebox_crazy_eights: ../open_spiel_whitebox.cc buggy_crazy_eights.cc
	clang++ -I"$(OPEN_SPIEL_PATH)" -I"$(OPEN_SPIEL_PATH)/open_spiel/abseil-cpp" -std=c++17 \
			-L"$(OPEN_SPIEL_PATH)/build" -lopen_spiel -fsanitize=fuzzer,address \
			-o build/open_spiel_whitebox_crazy_eights ../open_spiel_whitebox.cc -DGAME="buggy_crazy_eights(reshuffle=true,use_special_cards=false,players=2,max_draw_cards=3)" buggy_crazy_eights.cc -D__NUM_RANKS__=$(BUG_DEPTH)

build/open_spiel_blackbox_crazy_eights: ../open_spiel_blackbox.cc buggy_crazy_eights.cc
	clang++ -g -I"$(OPEN_SPIEL_PATH)" -I"$(OPEN_SPIEL_PATH)/open_spiel/abseil-cpp" -std=c++17 \
			-L"$(OPEN_SPIEL_PATH)/build" -lopen_spiel -fsanitize=fuzzer,address \
			-o build/open_spiel_blackbox_crazy_eights ../open_spiel_blackbox.cc -DGAME="buggy_crazy_eights(reshuffle=true,use_special_cards=false,players=2,max_draw_cards=3)" buggy_crazy_eights.cc -D__NUM_RANKS__=$(BUG_DEPTH)

clean:
	rm -f build/*