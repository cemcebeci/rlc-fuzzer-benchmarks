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

#include "buggy_blackjack.h"

#include <sys/types.h>

#include <string>
#include <utility>

#include "open_spiel/game_parameters.h"
#include "open_spiel/spiel.h"
#include "open_spiel/spiel_globals.h"

#include "../expected_error.h"

namespace open_spiel {
namespace buggy_blackjack {

namespace {
// Moves.
enum ActionType { kHit = 0, kStand = 1 };

constexpr int kPlayerId = 0;
constexpr int kAceValue = 1;
// The max score to approach for any player, i.e. as close to this as possible
// without exceeding it.
constexpr int kApproachScore = 21 * __TARGET_SCORE_MULTIPLIER__;
constexpr int kInitialCardsPerPlayer = 2;

// Facts about the game
const GameType kGameType{/*short_name=*/"buggy_blackjack",
                         /*long_name=*/"Blackjack",
                         GameType::Dynamics::kSequential,
                         GameType::ChanceMode::kExplicitStochastic,
                         GameType::Information::kImperfectInformation,
                         GameType::Utility::kGeneralSum,
                         GameType::RewardModel::kTerminal,
                         /*max_num_players=*/1,
                         /*min_num_players=*/1,
                         /*provides_information_state_string=*/false,
                         /*provides_information_state_tensor=*/false,
                         /*provides_observation_string=*/true,
                         /*provides_observation_tensor=*/true,
                         /*parameter_specification=*/{}};

static std::shared_ptr<const Game> Factory(const GameParameters& params) {
  return std::shared_ptr<const Game>(new BuggyBlackjackGame(params));
}

REGISTER_SPIEL_GAME(kGameType, Factory);

RegisterSingleTensorObserver single_tensor(kGameType.short_name);
}  // namespace

std::string BuggyBlackjackState::ActionToString(Player player,
                                           Action move_id) const {
  if (player == kChancePlayerId) {
    const char kSuitNames[kNumSuits + 1] = "CDHS";
    const char kRanks[kCardsPerSuit + 1] = "A23456789TJQK";
    return std::string(1, kSuitNames[move_id / kCardsPerSuit]) +
           std::string(1, kRanks[move_id % kCardsPerSuit]);
  } else if (move_id == ActionType::kHit) {
    return "Hit";
  } else {
    return "Stand";
  }
}

bool BuggyBlackjackState::IsTerminal() const { return turn_over_[DealerId()]; }

int BuggyBlackjackState::DealerId() const { return game_->NumPlayers(); }

std::vector<double> BuggyBlackjackState::Returns() const {
  if (!IsTerminal()) {
    return {0};
  }

  int player_total = GetBestPlayerTotal(kPlayerId);
  int dealer_total = GetBestPlayerTotal(DealerId());
  if (player_total > kApproachScore) {
    // Bust.
    return {-1};
  } else if (dealer_total > kApproachScore) {
    // Bust.
    return {+1};
  } else if (player_total > dealer_total) {
    return {+1};
  } else if (player_total < dealer_total) {
    return {-1};
  } else {
    // Tie.
    return {0};
  }
}

std::string BuggyBlackjackState::ObservationString(Player player) const {
  SPIEL_CHECK_GE(player, 0);
  SPIEL_CHECK_LT(player, game_->NumPlayers());
  return ToString();
}

void BuggyBlackjackState::ObservationTensor(Player player,
                                       absl::Span<float> values) const {
  std::fill(values.begin(), values.end(), 0);
  int offset = 0;

  // Whose turn is it?
  if (cur_player_ + 1 >= 0) {     // do not support kTerminalPlayerId
    values[cur_player_ + 1] = 1;  // to support kChancePlayerId (equals to -1)
  }
  offset += game_->NumPlayers() + 1;

  // Terminal?
  values[offset] = IsTerminal();
  offset += 1;

  // Number of aces for each player (incl. dealer)
  for (std::size_t player_id = 0; player_id < cards_.size(); player_id++) {
    values[offset + num_aces_[player_id]] = 1.0;
    offset += (kNumSuits + 1);
  }

  // Cards used by each player (incl. dealer)
  for (std::size_t player_id = 0; player_id < cards_.size(); player_id++) {
    for (const int& card : cards_[player_id]) {
      values[offset + card] = 1;
    }
    offset += kDeckSize;
  }

  SPIEL_CHECK_EQ(offset, values.size());
}

bool BuggyBlackjackState::InitialCardsDealt(int player) const {
  return cards_[player].size() >= kInitialCardsPerPlayer;
}

int BuggyBlackjackState::CardValue(int card) const {
  // Cards are indexed from 0 to kDeckSize-1;
  const int rank = card % kCardsPerSuit;
  if (rank == 0) {
    return kAceValue;
  } else if (rank <= 9) {
    return rank + 1;
  } else {
    // Ten or a face card.
    return 10;
  }
}

void BuggyBlackjackState::DealCardToPlayer(int player, int card) {
  // Remove card from deck.
  auto new_end = std::remove(deck_.begin(), deck_.end(), card);
  if (new_end == deck_.end()) throw ExpectedError();
  deck_.erase(new_end, deck_.end());

  cards_[player].push_back(card);
  const int value = CardValue(card);
  if (value == kAceValue) {
    num_aces_[player]++;
  } else {
    non_ace_total_[player] += value;
  }
}

BuggyBlackjackState::BuggyBlackjackState(std::shared_ptr<const Game> game) : State(game) {
  total_moves_ = 0;
  cur_player_ = kChancePlayerId;
  turn_player_ = kPlayerId;
  live_players_ = 1;

  // The values are stored for the dealer as well, whose id is NumPlayers.
  // See DealerId().
  non_ace_total_.resize(game_->NumPlayers() + 1, 0);
  num_aces_.resize(game_->NumPlayers() + 1, 0);
  turn_over_.resize(game_->NumPlayers() + 1, false);
  cards_.resize(game_->NumPlayers() + 1);

  deck_.resize(kDeckSize);
  std::iota(deck_.begin(), deck_.end(), 0);
}

int BuggyBlackjackState::GetBestPlayerTotal(int player) const {
  // Return the max possible total <= kApproachScore, depending on hard or soft
  // aces. 'Best' refers to the max non-bust score possible for the player.
  // If it is not possible, some value > kApproachScore is returned.
  int total = non_ace_total_[player] + num_aces_[player];
  for (int i = 1; i <= num_aces_[player]; i++) {
    int soft_total =
        non_ace_total_[player] + i * 11 + (num_aces_[player] - i) * 1;
    if (soft_total <= kApproachScore) {
      total = std::max(total, soft_total);
    }
  }
  /* -------------------------------------------------------------------------- */
  /*                       THIS IS THE BUG I'VE INTRODUCED                      */
  /* -------------------------------------------------------------------------- */
  if(total == kApproachScore)
    throw InsertedError();
  /* -------------------------------------------------------------------------- */
  return total;
}

int BuggyBlackjackState::CurrentPlayer() const { return cur_player_; }

int BuggyBlackjackState::NextTurnPlayer() const {
  if (IsTerminal()) {
    return kTerminalPlayerId;
  }
  return turn_over_[kPlayerId] ? DealerId() : kPlayerId;
}

void BuggyBlackjackState::EndPlayerTurn(int player) {
  turn_over_[player] = true;
  turn_player_ = NextTurnPlayer();
  cur_player_ = turn_player_;
}

void BuggyBlackjackState::DoApplyAction(Action move) {
  SPIEL_CHECK_EQ(IsTerminal(), false);

  if (!InitialCardsDealt(DealerId())) {
    // Still in the initial dealing phase. Deal the 'move' card to turn_player_.
    SPIEL_CHECK_EQ(IsChanceNode(), true);

    DealCardToPlayer(turn_player_, move);
    cur_player_ = kChancePlayerId;
    if (InitialCardsDealt(turn_player_)) {
      // Next player.
      turn_player_++;
      if (InitialCardsDealt(DealerId())) {
        // Hit/stand part of the game commences.
        turn_player_ = kPlayerId;
        cur_player_ = kPlayerId;
      }
    }
    return;
  }

  if (IsChanceNode()) {
    // Deal the 'move' card to turn_player_.
    DealCardToPlayer(turn_player_, move);
    cur_player_ = turn_player_;
    if (GetBestPlayerTotal(turn_player_) > kApproachScore) {
      if (turn_player_ != DealerId()) --live_players_;
      EndPlayerTurn(turn_player_);
    }
    MaybeApplyDealerAction();
    return;
  }

  total_moves_++;
  if (move == kHit) {
    cur_player_ = kChancePlayerId;
  } else if (move == kStand) {
    EndPlayerTurn(turn_player_);
    MaybeApplyDealerAction();
  }
}

void BuggyBlackjackState::MaybeApplyDealerAction() {
  // If there are no players still live, dealer doesn't play.
  if (live_players_ == 0) {
    EndPlayerTurn(DealerId());
  }

  // Otherwise, hits 16 or less, stands on 17 or more.
  if (cur_player_ == DealerId()) {
    if (GetBestPlayerTotal(DealerId()) <= kApproachScore - 5) {
      cur_player_ = kChancePlayerId;
    } else {
      EndPlayerTurn(cur_player_);
    }
  }
}

std::vector<Action> BuggyBlackjackState::LegalActions() const {
  SPIEL_CHECK_NE(cur_player_, DealerId());
  if (IsChanceNode()) {
    return LegalChanceOutcomes();
  } else if (IsTerminal()) {
    return {};
  } else {
    return {kHit, kStand};
  }
}

ActionsAndProbs BuggyBlackjackState::ChanceOutcomes() const {
  SPIEL_CHECK_TRUE(IsChanceNode());
  ActionsAndProbs outcomes;
  outcomes.reserve(deck_.size());
  for (int card : deck_) {
    outcomes.emplace_back(card, 1.0 / deck_.size());
  }
  return outcomes;
}

std::string BuggyBlackjackState::ToString() const {
  return absl::StrCat("Non-Ace Total: ", absl::StrJoin(non_ace_total_, " "),
                      " Num Aces: ", absl::StrJoin(num_aces_, " "),
                      (cur_player_ == kChancePlayerId ? ", Chance Player\n"
                                                      : ", Player's Turn\n"));
}

std::unique_ptr<State> BuggyBlackjackState::Clone() const {
  return std::unique_ptr<State>(new BuggyBlackjackState(*this));
}

BuggyBlackjackGame::BuggyBlackjackGame(const GameParameters& params)
    : Game(kGameType, params) {}

}  // namespace blackjack
}  // namespace open_spiel
