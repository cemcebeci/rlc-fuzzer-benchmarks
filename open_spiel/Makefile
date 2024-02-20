all: build/tic_tac_toe_fuzzer build/blackjack_fuzzer build/crazy_eights_fuzzer

build/tic_tac_toe_fuzzer: fuzz_target.cc buggy_tic_tac_toe/buggy_tic_tac_toe.cc
	clang++ -I"${OPEN_SPIEL_PATH}" -I"${OPEN_SPIEL_PATH}/open_spiel/abseil-cpp" -std=c++17 \
                -L"${OPEN_SPIEL_PATH}/build" -lopen_spiel -fsanitize=fuzzer,address \
                -o build/tic_tac_toe_fuzzer fuzz_target.cc -DGAME=buggy_tic_tac_toe buggy_tic_tac_toe/buggy_tic_tac_toe.cc

build/blackjack_fuzzer: fuzz_target.cc buggy_blackjack/buggy_blackjack.cc
	clang++ -I"${OPEN_SPIEL_PATH}" -I"${OPEN_SPIEL_PATH}/open_spiel/abseil-cpp" -std=c++17 \
                -L"${OPEN_SPIEL_PATH}/build" -lopen_spiel -fsanitize=fuzzer,address \
                -o build/blackjack_fuzzer fuzz_target.cc -DGAME=buggy_blackjack buggy_blackjack/buggy_blackjack.cc

build/crazy_eights_fuzzer: fuzz_target.cc buggy_crazy_eights/crazy_eights.cc
    clang++ -I"${OPEN_SPIEL_PATH}" -I"${OPEN_SPIEL_PATH}/open_spiel/abseil-cpp" -std=c++17 \
                -L"${OPEN_SPIEL_PATH}/build" -lopen_spiel -fsanitize=fuzzer,address \
                -o build/crazy_eights_fuzzer fuzz_target.cc -DGAME=buggy_crazy_eights buggy_crazy_eights/crazy_eights.cc

clean:
    rm build/*