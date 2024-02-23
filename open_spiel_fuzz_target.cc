#include <cstring>
#include <memory>
#include <vector>

#include "open_spiel/spiel.h"
#include "open_spiel/spiel_utils.h"

#ifdef DEBUG
#define out std::cout
#else
#define out std::ostream(nullptr)
#endif

void PrintLegalActions(const open_spiel::State &state,
                       open_spiel::Player player,
                       const std::vector<open_spiel::Action> &movelist) {
  out << "Legal moves for player " << player << ":" << std::endl;
  for (open_spiel::Action action : movelist) {
    out << "  " << state.ActionToString(player, action) << std::endl;
  }
}

class OutOfFuzzInputException : public std::exception {};

static int64_t consume_bits(const char *Data, size_t Size, int num_bits, int &byte_offset,
                            int &bit_offset) {
  int64_t result = 0;
  int remaining_bits = num_bits;

  while (true) {
    if(byte_offset >= Size) {
      throw OutOfFuzzInputException();
    }
    int to_consume_from_current_byte = std::min(remaining_bits, 8 - bit_offset);
    int shift_count = (8 - bit_offset - remaining_bits);
    int mask = ((1u << to_consume_from_current_byte) - 1) << shift_count;
    int data = (*(Data + byte_offset) & mask) >> shift_count;
    result = (result << to_consume_from_current_byte) | data;

    if (remaining_bits >= (8 - bit_offset)) {
      byte_offset++;
      remaining_bits -= (8 - bit_offset);
      bit_offset = 0;
    } else {
      bit_offset = bit_offset + remaining_bits;
      return result;
    }
  }
}

static open_spiel::Action pickAction(std::vector<open_spiel::Action> actions,
                                     const char *Data, size_t Size,
                                     int &byte_offset, int &bit_offset) {
  int num_actions = actions.size();
  int num_bits = ceil(log2(num_actions));
  auto picked_index =
      std::abs(consume_bits(Data, Size, num_bits, byte_offset, bit_offset) % num_actions);
  out << "picked: " << picked_index << "\n";
  return actions.at(picked_index);
}

#ifndef GAME
#define GAME tic_tac_toe
#endif
#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)

extern "C" int LLVMFuzzerTestOneInput(const char *Data, size_t Size) {
  int byte_offset = 0, bit_offset = 0;

  out << "Loading game..\n" << std::endl;
  std::shared_ptr<const open_spiel::Game> game =
      open_spiel::LoadGame(STR(GAME));

  if (!game) {
    out << "problem with loading game, exiting..." << std::endl;
    return -1;
  }

  out << "Starting new game..." << std::endl;
  std::unique_ptr<open_spiel::State> state = game->NewInitialState();

  out << "Initial state:" << std::endl;
  out << "State:" << std::endl << state->ToString() << std::endl;

  try {
    while (!state->IsTerminal()) {
      // Assuming no simultaneous nodes.
      open_spiel::Action action = pickAction(state->LegalActions(), Data,
                                              Size, byte_offset, bit_offset);
      state->ApplyAction(action);
      out << "State: " << std::endl << state->ToString() << std::endl;
    }
  } catch (OutOfFuzzInputException e) {
    return 0;
  }

  auto returns = state->Returns();
  for (auto p = open_spiel::Player{0}; p < game->NumPlayers(); p++) {
    out << "Final return to player " << p << " is " << returns[p]
              << std::endl;
  }
  return 0;
}
