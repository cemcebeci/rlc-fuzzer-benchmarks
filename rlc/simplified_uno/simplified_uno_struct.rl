import fuzzer.cpp_functions
import fuzzer.utils

fun crash() {false}:
    return

ent Card:
    Int suit
    Int rank

act play() -> Uno:
    frm last_card : Card

    act pick_initial_card( Card card) { card.rank >= 0, card.rank < 13, card.rank != 8, card.suit >= 0, card.suit < 4}
    last_card = card

    frm i = 0
    while i < 4:
        act play_card(Card card) {
            card.rank >= 0,
            card.rank < 13,
            card.suit >= 0,
            card.suit < 4,
            card.rank == 8 or ((card.suit == last_card.suit) or (card.rank == last_card.rank))
        }
        last_card = card

        i = i + 1
    crash()
