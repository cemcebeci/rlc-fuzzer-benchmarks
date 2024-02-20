import fuzzer.cpp_functions
import fuzzer.utils

fun crash() {false}:
    return

act play() -> Uno:
    frm last_suit : Int
    frm last_rank : Int

    act pick_initial_card( Int rank, Int suit) { rank >= 0, rank < 13, rank != 8, suit >= 0, suit < 4}
    last_suit = suit
    last_rank = rank

    frm i = 0
    while i < 4:
        act play_card( Int rank, Int suit) {
            rank >= 0,
            rank < 13,
            suit >= 0,
            suit < 4,
            rank == 8 or ((suit == last_suit) or (rank == last_rank))
        }
        last_suit = suit
        last_rank = rank

        i = i + 1
    crash()
