#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

#include "board.h"
#include "color.h"
#include "engine.h"
#include "move.h"

static int multiplier(Color color) {
  return color == kWhite ? 1 : -1;
}

ColorfulCompare::ColorfulCompare(Color color) {
  multiplier_ = multiplier(color);
}

bool ColorfulCompare::operator()(const Move& a, const Move& b) {
  return (multiplier_ * a.Utility()) < (multiplier_ * b.Utility());
}

GameOutcome GetGameOutcome(bool has_valid_moves, bool isCheck) {
  if (has_valid_moves) {
    return kInProgress;
  } else {
    return isCheck ? kCheckmate : kDraw;
  }
}

double MaterialisticUtility(Board& board, GameOutcome outcome, Color attackingcolor) {
  switch (outcome) {
    case kCheckmate:
      return multiplier(attackingcolor) * std::numeric_limits<double>::infinity();
    case kDraw:
      return 0.0;
    default:
      float utility = 0;

      for (auto piece : board.GetPieces()) {
        utility += piece->Value() * multiplier(piece->GetColor());
      }

      return utility;
  }
}

double SmartUtility(Board& board, GameOutcome outcome, Color attackingcolor) {
  switch (outcome) {
    case kCheckmate:
      return multiplier(attackingcolor) * std::numeric_limits<double>::infinity();
    case kDraw:
      return 0.0;
    default:
      float utility = 0;
      float my_value = 0;
      float their_value = 0;

      for (auto piece : board.GetPieces()) {
        utility += piece->Value() * multiplier(piece->GetColor());
        if (piece->GetColor() == attackingcolor) {
          my_value += piece->Value();
        } else {
          their_value += piece->Value();
        }
      }

      float space = std::pow(board.GetMoves(attackingcolor).size(), 0.05);
      float ratio = std::pow(my_value / their_value, 0.05);

      return utility * space * ratio;
  }
}

bool IsUtilityBetterThan(float a, float b, Color color) {
  auto x = multiplier(color);
  return a * x > b * x;
}

class CapturesFirst {
 public:
  CapturesFirst(const Board& board) : board_(board) {}
  bool operator()(const Move& a, const Move& b) {
    auto to_a = board_.GetPiece(a.To());
    auto to_b = board_.GetPiece(b.To());
    return to_a == nullptr && to_b != nullptr || (to_a != nullptr && to_b != nullptr && to_a->Value() < to_b->Value());
  }
 private:
  const Board& board_;
};

void ComputeUtilityInternal(Board& board, Color mycolor, int depth, float theirbest, double (*utility)(Board& board, GameOutcome outcome, Color attackingcolor), std::vector<Move>& moves) {
  auto theircolour = Other(mycolor);
  float mybest = multiplier(theircolour) * std::numeric_limits<double>::infinity();
  int c = 0;
  std::sort(moves.rbegin(), moves.rend(), CapturesFirst(board));
  for (auto it = moves.begin(); it != moves.end(); ++it) {
    std::unique_ptr<Board::Snapshot> snapshot = board.TakeSnapshot();
    board.DoMove(*it);
    std::vector<Move> theirmoves = board.GetMoves(theircolour);
    bool ischeck = board.IsCheck(theircolour);
    GameOutcome outcome = GetGameOutcome(!theirmoves.empty(), ischeck);
    if (depth == 0 || outcome != kInProgress) {
      it->SetUtility(utility(board, outcome, mycolor));
    } else {
      ComputeUtilityInternal(board, theircolour, depth-1, mybest, utility, theirmoves);
      auto theirbest = std::max_element(theirmoves.begin(), theirmoves.end(), ColorfulCompare(theircolour));
      it->SetUtility(theirbest->Utility());
    }
    board.RecoverSnapshot(*snapshot);
    if (IsUtilityBetterThan(it->Utility(), mybest, mycolor)) {
      mybest = it->Utility();
    }
    ++c;
    if (IsUtilityBetterThan(theirbest, it->Utility(), theircolour)) {
      // We might as well stop now as the calling function already has a better solution than this.
      break;
    }
  }
  if (c < moves.size()) {
    moves.erase(moves.begin() + c, moves.end());
  }
}

std::vector<Move> ComputeUtility(Board& board, Color mycolor, int depth, double (*utility)(Board& board, GameOutcome outcome, Color attackingcolor)) {
  std::vector<Move> moves = board.GetMoves(mycolor);
  ComputeUtilityInternal(board, mycolor, depth, multiplier(mycolor) * std::numeric_limits<double>::infinity(), utility, moves);
  return moves;
}