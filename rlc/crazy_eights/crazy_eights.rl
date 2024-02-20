import fuzzer.cpp_functions
import fuzzer.utils

ext fun p1_is_dealt(Int suit, Int rank)
ext fun p2_is_dealt(Int suit, Int rank)
ext fun revealed(Int suit, Int rank)
ext fun p1_plays(Int suit, Int rank)
ext fun p2_plays(Int suit, Int rank)

fun crash() {false}:
    return

ent Card:
    Int suit
    Int rank

fun init_deck(Vector<Card> deck):
    let i = 1
    while i <= 13 :
        let current : Card
        current.rank = i
        current.suit = 0
        deck.append(current)
        current.suit = 1
        deck.append(current)
        current.suit = 2
        deck.append(current)
        current.suit = 3
        deck.append(current)
        i = i + 1
    return

fun take_card_at_index(Vector<Card> deck, Int index) -> Card:
    let card = deck.get(index)
    deck.erase(index)
    return card

act play() -> CrazyEights:
    frm deck: Vector<Card>
    frm p_one_hand: Vector<Card>
    frm p_two_hand: Vector<Card>
    init_deck(deck)

    # deal the cards
    frm i = 0
    while i < 5:
        act deal(Int index) {index >= 0, index < deck.size()}
        let card = take_card_at_index(deck, index)
        p_one_hand.append(card)
        p1_is_dealt(card.suit, card.rank)
        
        act deal(Int index) {index >= 0, index < deck.size()}
        let card = take_card_at_index(deck, index)
        p_two_hand.append(card)
        p2_is_dealt(card.suit, card.rank)

        i = i + 1

    # reveal the top card
    frm last_card : Card
    act reveal_top_card(Int top_card_index) { top_card_index >= 0, top_card_index < deck.size()}
    last_card = take_card_at_index(deck, top_card_index)
    revealed(last_card.suit, last_card.rank)

    i = 0
    while i < 3:
        act p_one_plays(Int card_index_in_hand) {
            card_index_in_hand >= 0,
            card_index_in_hand < p_one_hand.size(),
            p_one_hand.get(card_index_in_hand).rank == 8 or 
            p_one_hand.get(card_index_in_hand).suit == last_card.suit or 
            p_one_hand.get(card_index_in_hand).rank == last_card.rank
        }
        last_card = take_card_at_index(p_one_hand, card_index_in_hand)
        p1_plays(last_card.suit, last_card.rank)

        act p_two_plays(Int card_index_in_hand) {
            card_index_in_hand >= 0,
            card_index_in_hand < p_two_hand.size(),
            p_two_hand.get(card_index_in_hand).rank == 8 or 
            p_two_hand.get(card_index_in_hand).suit == last_card.suit or 
            p_two_hand.get(card_index_in_hand).rank == last_card.rank
        }
        last_card = take_card_at_index(p_two_hand, card_index_in_hand)
        p2_plays(last_card.suit, last_card.rank)

        i = i + 1
    crash()
