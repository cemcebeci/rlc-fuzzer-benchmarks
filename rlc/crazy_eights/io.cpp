#include <cstdint>
#include <iostream>
#include <memory>

#ifndef NODEBUGINFO
#define out std::cout
#else
#define out std::ostream(nullptr)
#endif

extern "C" void rl_p1_is_dealt__int64_t_int64_t_r_void(std::uint64_t *suit, std::uint64_t *rank) {
    out << "p1 is dealt (" << *suit << ", " << *rank << ").\n";
}

extern "C" void rl_p2_is_dealt__int64_t_int64_t_r_void(std::uint64_t *suit, std::uint64_t *rank) {
    out << "p2 is dealt (" << *suit << ", " << *rank << ").\n";
}

extern "C" void rl_p1_plays__int64_t_int64_t_r_void(std::uint64_t *suit, std::uint64_t *rank) {
    out << "p1 plays (" << *suit << ", " << *rank << ").\n";
}

extern "C" void rl_p2_plays__int64_t_int64_t_r_void(std::uint64_t *suit, std::uint64_t *rank) {
    out << "p2 plays (" << *suit << ", " << *rank << ").\n";
}

extern "C" void rl_revealed__int64_t_int64_t_r_void(std::uint64_t *suit, std::uint64_t *rank) {
    out << "revealed (" << *suit << ", " << *rank << ").\n";
}

extern "C" void rl_p1_skip__r_void() {
    out << "p1 skips\n";
}

extern "C" void rl_p2_skip__r_void() {
    out << "p2 skips\n";
}

