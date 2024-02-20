#import fuzzer.cpp_functions
#import fuzzer.utils
import collections.vector

fun crash() {1 == 0}:
    return

ent Deck:
    Vector<Int> cards
    
    fun init():
        self.cards.init()
        let i = 1
        while i <= 13 :
            # once for each suit.
            self.cards.append(i)
            self.cards.append(i)
            self.cards.append(i)
            self.cards.append(i)
            i = i + 1

fun calculate_points(Vector<Int> hand) -> Int:
    let total = 0
    let num_ones = 0
    
    let i = 0
    while i < hand.size():
        let card = hand.get(i)
        if card <= 10:
            total = total + card
        else:
            total = total + 10
        
        if card == 1:
            num_ones = num_ones + 1
        i = i + 1

    while num_ones > 0 and total + 10 < 21:
        num_ones = num_ones - 1
        total = total + 10

    if num_ones == 2:
        crash()
    
    return total

act play() -> Blackjack:
    frm deck : Deck
    frm player_hand : Vector<Int>
    frm dealer_hand : Vector<Int>
    frm i = 0
    while i < 2:
        act deal_player(Int index) {index >= 0, index < deck.cards.size()}
        let card = deck.cards.get(index)
        deck.cards.erase(index)
        player_hand.append(card)
        
        act deal_dealer(Int index) {index >= 0, index < deck.cards.size()}
        let card = deck.cards.get(index)
        deck.cards.erase(index)
        dealer_hand.append(card)
        i = i + 1

    frm player_passed = false
    frm player_bust = false

    while !player_bust and !player_passed:
        actions:
            act hit()
            act draw_player(Int index) {index >= 0, index < deck.cards.size()}
            frm card = deck.cards.get(index)
            deck.cards.erase(index)
            player_hand.append(card)
            if calculate_points(player_hand) > 21:
                player_bust = true

            act stand()
            player_passed = true
    
    while calculate_points(dealer_hand) <= 16:
        act draw_dealer(Int index) {index >= 0, index < deck.cards.size()}
        frm card = deck.cards.get(index)
        deck.cards.erase(index)
        dealer_hand.append(card)

fun main() -> Int:
   let game = play()
   game.deal_player(4)

   if(game.player_hand.size() != 2):
       return 1
   if(game.dealer_hand.size() != 2):
       return 1
   game.hit()
   game.draw_player(2)

   if(!game.is_done()):
       return 1
   return 0
