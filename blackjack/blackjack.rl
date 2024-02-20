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

act draw(ctx Deck deck, ctx Vector<Int> hand) -> Draw:
    # BUG? deck.cards.size does not work in the condition.
    act draw(Int index) {index >= 0, index < deck.size()}
    frm card = deck.cards.get(index)
    deck.cards.erase(index)
    hand.append(card)

act deal(ctx Deck deck, ctx Vector<Int> hand) -> Deal:
    frm i = 0
    while i < 2:
        subaction* (deck, hand) d = draw(deck, hand)
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

    subaction* (deck, player_hand) deal_player = deal(deck, player_hand)
    subaction* (deck, player_hand) deal_dealer = deal(deck, dealer_hand)

    frm player_passed = false
    frm player_bust = false

    while !player_bust and !player_passed:
        actions:
            act hit()
            subaction* (deck, player_hand) player_draws = draw(deck, player_hand)
            if calculate_points(player_hand) > 21:
                player_bust = true

            act stand()
            player_passed = true
    
    while calculate_points(dealer_hand) <= 16:
        subaction* (deck, dealer_hand) dealer_draws = draw(deck, dealer_hand)

fun main() -> Int:
# BUG? This seems to get stuck in a loop.  
   let game = play()
   game.draw(4)
   game.draw(4)
   game.draw(4)
   game.draw(4)
   if(game.player_hand.size() != 2):
       return 1
   if(game.dealer_hand.size() != 2):
       return 1
   game.hit()

   if(!game.is_done()):
       return 1
   return 0
