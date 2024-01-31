#include "blackjack.cpp"
#include <cstdint>
#include <cstdio>
#include <iostream>

#define RLC_GET_FUNCTION_DECLS
#define RLC_GET_TYPE_DECLS

extern "C" {
    #include "build/rlc_names.h"
}

extern "C" void rl_Blackjack_new__int64_t_r_int64_t(int64_t * __result, int64_t * player_count) {
    *__result = (int64_t) (void *)new Blackjack(*player_count);
    std::cout << "Allocated: " << *__result << "\n";
}

extern "C" void rl_Blackjack_delete__int64_t_r_void(int64_t * address) {
    std::cout << "Deleted: " << *address << "\n";
    delete (Blackjack *)(void *)*address;
}

extern "C" void rl_Blackjack_shuffle__int64_t_r_void(int64_t *address) {
    ((Blackjack *)(void *)*address)->shuffle_deck();
}

extern "C" void rl_Blackjack_deal__int64_t_r_void(int64_t *address) {
    ((Blackjack *)(void *)*address)->deal();
}

extern "C" void rl_Blackjack_draw__int64_t_int64_t_r_void(int64_t * address, int64_t * player) {
    ((Blackjack *)(void *)*address)->take_card(*player);
}

extern "C" void rl_Blackjack_pass__int64_t_int64_t_r_void(int64_t * address, int64_t * player) {
    ((Blackjack *)(void *)*address)->pass(*player);
}

extern "C" void rl_Blackjack_dump_state__int64_t_r_void(int64_t * address) {
    ((Blackjack *)(void *)*address)->dump_state();
}

extern "C" void rl_Blackjack_calculate_points__int64_t_int64_t_r_int64_t(int64_t *__result, int64_t *address, int64_t *player) {
    *__result = ((Blackjack *)(void *)*address)->calculate_points(*player);
}