// Copyright 2019 DeepMind Technologies Limited
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "buggy_tic_tac_toe.h"
#include "../expected_error.h"

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "open_spiel/spiel_utils.h"
#include "open_spiel/utils/tensor_view.h"

namespace open_spiel {
namespace buggy_tic_tac_toe {
namespace {

// Facts about the game.
const GameType kGameType{
    /*short_name=*/"buggy_tic_tac_toe",
    /*long_name=*/"Tic Tac Toe",
    GameType::Dynamics::kSequential,
    GameType::ChanceMode::kDeterministic,
    GameType::Information::kPerfectInformation,
    GameType::Utility::kZeroSum,
    GameType::RewardModel::kTerminal,
    /*max_num_players=*/2,
    /*min_num_players=*/2,
    /*provides_information_state_string=*/true,
    /*provides_information_state_tensor=*/false,
    /*provides_observation_string=*/true,
    /*provides_observation_tensor=*/true,
    /*parameter_specification=*/{}  // no parameters
};

std::shared_ptr<const Game> Factory(const GameParameters& params) {
  return std::shared_ptr<const Game>(new BuggyTicTacToeGame(params));
}

REGISTER_SPIEL_GAME(kGameType, Factory);

RegisterSingleTensorObserver single_tensor(kGameType.short_name);

}  // namespace

CellState PlayerToState(Player player) {
  switch (player) {
    case 0:
      return CellState::kCross;
    case 1:
      return CellState::kNought;
    default:
      SpielFatalError(absl::StrCat("Invalid player id ", player));
      return CellState::kEmpty;
  }
}

std::string StateToString(CellState state) {
  switch (state) {
    case CellState::kEmpty:
      return ".";
    case CellState::kNought:
      return "o";
    case CellState::kCross:
      return "x";
    default:
      SpielFatalError("Unknown state.");
  }
}

bool BoardHasLine(const std::array<CellState, kNumCells>& board,
                  const Player player) {
  CellState c = PlayerToState(player);
  // check rows
  for (int i = 0; i < kNumRows; i ++) {
    bool allMarked = true;
    for (int j = 0; j < kNumCols; j ++) {
      if(board[i * kNumCols + j] != c) {
        allMarked = false;
        break;
      }
    }
    if(allMarked) return true;
  }
  // check columns
  for (int j = 0; j < kNumCols; j ++) {
    bool allMarked = true;
    for (int i = 0; j < kNumRows; i ++) {
      if(board[i * kNumCols + j] != c) {
        allMarked = false;
        break;
      }
    }
    if(allMarked) return true;
  }
  //check diagonals
  bool allMarked = true;
  for(int i = 0; i < kNumRows; i++) {
    int j = i;
    if(board[i * kNumCols + j] != c) {
      allMarked = false;
      break;
    }
  }
  if(allMarked) return true;

  allMarked = true;
  for(int i = 0; i < kNumRows; i++) {
    int j = kNumRows - 1 - i;
    if(board[i * kNumCols + j] != c) {
      allMarked = false;
      break;
    }
  }
  if(allMarked){
    return true;
  } 

  return false;
}

void BuggyTicTacToeState::DoApplyAction(Action move) {
  if(move >= kNumCells || move < 0 || board_[move] != CellState::kEmpty) {
    throw ExpectedError();
  }
  SPIEL_CHECK_EQ(board_[move], CellState::kEmpty);
  board_[move] = PlayerToState(CurrentPlayer());
  if (HasLine(current_player_)) {
    outcome_ = current_player_;
    /* -------------------------------------------------------------------------- */
    /*                       THIS IS THE BUG I'VE INTRODUCED                      */
    /* -------------------------------------------------------------------------- */
      if(BoardAt(0,2) == BoardAt(1,1) and
        BoardAt(0,2) == BoardAt(2,0) and
        BoardAt(0,2) == PlayerToState(current_player_))
        throw InsertedError();
    /* -------------------------------------------------------------------------- */
  }
  current_player_ = 1 - current_player_;
  num_moves_ += 1;
}

std::vector<Action> BuggyTicTacToeState::LegalActions() const {
  if (IsTerminal()) return {};
  // Can move in any empty cell.
  std::vector<Action> moves;
  for (int cell = 0; cell < kNumCells; ++cell) {
    if (board_[cell] == CellState::kEmpty) {
      moves.push_back(cell);
    }
  }
  return moves;
}

std::string BuggyTicTacToeState::ActionToString(Player player,
                                           Action action_id) const {
  return game_->ActionToString(player, action_id);
}

bool BuggyTicTacToeState::HasLine(Player player) const {
  return BoardHasLine(board_, player);
}

bool BuggyTicTacToeState::IsFull() const { return num_moves_ == kNumCells; }

BuggyTicTacToeState::BuggyTicTacToeState(std::shared_ptr<const Game> game) : State(game) {
  std::fill(begin(board_), end(board_), CellState::kEmpty);
}

std::string BuggyTicTacToeState::ToString() const {
  std::string str;
  for (int r = 0; r < kNumRows; ++r) {
    for (int c = 0; c < kNumCols; ++c) {
      absl::StrAppend(&str, StateToString(BoardAt(r, c)));
    }
    if (r < (kNumRows - 1)) {
      absl::StrAppend(&str, "\n");
    }
  }
  return str;
}

bool BuggyTicTacToeState::IsTerminal() const {
  return outcome_ != kInvalidPlayer || IsFull();
}

std::vector<double> BuggyTicTacToeState::Returns() const {
  if (HasLine(Player{0})) {
    return {1.0, -1.0};
  } else if (HasLine(Player{1})) {
    return {-1.0, 1.0};
  } else {
    return {0.0, 0.0};
  }
}

std::string BuggyTicTacToeState::InformationStateString(Player player) const {
  SPIEL_CHECK_GE(player, 0);
  SPIEL_CHECK_LT(player, num_players_);
  return HistoryString();
}

std::string BuggyTicTacToeState::ObservationString(Player player) const {
  SPIEL_CHECK_GE(player, 0);
  SPIEL_CHECK_LT(player, num_players_);
  return ToString();
}

void BuggyTicTacToeState::ObservationTensor(Player player,
                                       absl::Span<float> values) const {
  SPIEL_CHECK_GE(player, 0);
  SPIEL_CHECK_LT(player, num_players_);

  // Treat `values` as a 2-d tensor.
  TensorView<2> view(values, {kCellStates, kNumCells}, true);
  for (int cell = 0; cell < kNumCells; ++cell) {
    view[{static_cast<int>(board_[cell]), cell}] = 1.0;
  }
}

void BuggyTicTacToeState::UndoAction(Player player, Action move) {
  board_[move] = CellState::kEmpty;
  current_player_ = player;
  outcome_ = kInvalidPlayer;
  num_moves_ -= 1;
  history_.pop_back();
  --move_number_;
}

std::unique_ptr<State> BuggyTicTacToeState::Clone() const {
  return std::unique_ptr<State>(new BuggyTicTacToeState(*this));
}

std::string BuggyTicTacToeGame::ActionToString(Player player,
                                          Action action_id) const {
  return absl::StrCat(StateToString(PlayerToState(player)), "(",
                      action_id / kNumCols, ",", action_id % kNumCols, ")");
}

BuggyTicTacToeGame::BuggyTicTacToeGame(const GameParameters& params)
    : Game(kGameType, params) {}

}  // namespace tic_tac_toe
}  // namespace open_spiel
