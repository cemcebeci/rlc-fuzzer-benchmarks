#include <cstring>
#include <memory>
#include <vector>

#include "open_spiel/spiel.h"
#include "open_spiel/spiel_utils.h"
#include "open_spiel/spiel_globals.h"
#include "expected_error.h"

#ifdef DEBUG
#define out std::cout
#else
#define out std::ostream(nullptr)
#endif

class OutOfFuzzInputException : public std::exception {};

static int64_t consume_bits(const char *Data, size_t Size, int num_bits, int &byte_offset, int &bit_offset) {
    int64_t result = 0;
    int remaining_bits = num_bits;

    while (true) {
        if(byte_offset >= Size) {
          throw OutOfFuzzInputException();
        }
        int to_consume_from_current_byte = std::min(remaining_bits, 8 - bit_offset);
        int shift_count = bit_offset;
        int mask = ((1u << to_consume_from_current_byte) - 1) << shift_count;
        int data = (*(Data + byte_offset) & mask) >> shift_count;
        result = (result << to_consume_from_current_byte) | data;
        if(remaining_bits > to_consume_from_current_byte) {
            byte_offset ++;
            remaining_bits -= to_consume_from_current_byte;
            bit_offset = 0;
        } else {
            bit_offset = bit_offset + to_consume_from_current_byte % 8;
            return result;
        }
    }
}

#ifndef GAME
#define GAME tic_tac_toe
#endif
#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)

#ifndef __NUM_GAMES__
#define __NUM_GAMES__ 1
#endif

extern "C" int LLVMFuzzerTestOneInput(const char *Data, size_t Size) {
  int byte_offset = 0, bit_offset = 0;

  out << "Loading game..\n" << std::endl;
  std::shared_ptr<const open_spiel::Game> game =
      open_spiel::LoadGame(STR(GAME));

  if (!game) {
    out << "problem with loading game, exiting..." << std::endl;
    return -1;
  }
  for(int i = 1; i <= __NUM_GAMES__; i++) {
    out << "Starting new game..." << std::endl;
    std::unique_ptr<open_spiel::State> state = game->NewInitialState();

    out << "Initial state:" << std::endl;
    out << "State:" << std::endl << state->ToString() << std::endl;

    try {
      while (!state->IsTerminal()) {
        // Assuming no simultaneous nodes.
        open_spiel::Action action = consume_bits(Data, Size, 64, byte_offset, bit_offset);
        // avoid the invalid action.
        while(action == open_spiel::kInvalidAction) {
          consume_bits(Data, Size, 64, byte_offset, bit_offset);
        }
        out << "picked: " << action << "\n";
        state->ApplyAction(action);
        out << "State: " << std::endl << state->ToString() << std::endl;
      }
    } catch (OutOfFuzzInputException e) {
      return 0;
    } catch (ExpectedError e) {
      return 0;
    } catch (InsertedError e) {
      if (i < __NUM_GAMES__) {
        // carry on with the next game
      } else {
        std::cout << "Found the bug.\n";
        __builtin_trap();
      }
    }
  }
  return 0;
}
