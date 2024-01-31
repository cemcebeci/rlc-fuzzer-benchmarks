import fuzzer.cpp_functions
import fuzzer.utils

ext fun Blackjack_new(Int player_count) -> Int
ext fun Blackjack_delete(Int address)
ext fun Blackjack_shuffle(Int address)
ext fun Blackjack_deal(Int address)
ext fun Blackjack_draw(Int address, Int player)
ext fun Blackjack_pass(Int address, Int player)
ext fun Blackjack_dump_state(Int address)
ext fun Blackjack_calculate_points(Int address, Int player) -> Int

ent BlackjackImplementation:
    Int address
    
    fun init():
        self.address = Blackjack_new(2)

    fun drop():
        Blackjack_delete(self.address)

act play() -> Jack:
    let blackjack : BlackjackImplementation

    Blackjack_shuffle(blackjack.address)
    Blackjack_deal(blackjack.address)

    let passed = false
    let bust = false
    while !bust:
        actions:
            act hit()
            Blackjack_draw(blackjack.address, 0)
            if Blackjack_calculate_points(blackjack.address, 0) == -1:
                bust = true

            act stand()
            passed = true
    Blackjack_pass(blackjack.address, 0)
    
    while Blackjack_calculate_points(blackjack.address, 1) <= 16 and Blackjack_calculate_points(blackjack.address, 1) != -1:
        Blackjack_draw(blackjack.address, 1)

    Blackjack_pass(blackjack.address, 1)
