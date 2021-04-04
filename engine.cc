#include <algorithm>
#include <iostream>
#include <limits>

#include "board.h"
#include "color.h"
#include "engine.h"
#include "move.h"

int multiplier(Color color) {
  return color == kWhite ? 1 : -1;
}

Color switch_color(Color color) {
  return color == kWhite ? kBlack : kWhite;
}

GameOutcome GetGameOutcome(bool has_valid_moves, bool isCheck) {
  if (has_valid_moves) {
    return kInProgress;
  } else {
    return isCheck ? kCheckmate : kDraw;
  }
}

double MaterialisticUtility(const Board& board, GameOutcome outcome, Color attackingcolor) {
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

void ComputeUtilityInternal(Board& board, Color mycolor, int depth, double (*utility)(const Board& board, GameOutcome outcome, Color attackingcolor), std::vector<Move>& moves) {
  auto theircolour = switch_color(mycolor);
  for (auto it = moves.begin(); it != moves.end(); ++it) {
    std::unique_ptr<Board::Snapshot> snapshot = board.TakeSnapshot();
    board.DoMove(*it);
    std::vector<Move> theirmoves = board.GetMoves(theircolour);
    bool ischeck = board.IsCheck(theircolour);
    GameOutcome outcome = GetGameOutcome(!theirmoves.empty(), ischeck);
    if (depth == 0 || outcome != kInProgress) {
      it->SetUtility(utility(board, outcome, mycolor));
    } else {
      ComputeUtilityInternal(board, theircolour, depth-1, utility, theirmoves);
      auto theirbest = std::max_element(theirmoves.begin(), theirmoves.end(), MultiplierCompare(multiplier(theircolour)));
      it->SetUtility(theirbest->Utility());
    }
    board.RecoverSnapshot(*snapshot);
  }
}

std::vector<Move> ComputeUtility(Board& board, Color mycolor, int depth, double (*utility)(const Board& board, GameOutcome outcome, Color attackingcolor)) {
  std::vector<Move> moves = board.GetMoves(mycolor);
  ComputeUtilityInternal(board, mycolor, depth, utility, moves);
  return moves;
}