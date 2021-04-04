#ifndef ENGINE_H_
#define ENGINE_H_
#include <vector>

#include "board.h"
#include "color.h"

enum GameOutcome { kInProgress, kDraw, kCheckmate };

GameOutcome GetGameOutcome(bool has_valid_moves, bool isCheck);

std::vector<Move> ComputeUtility(Board& board, Color mycolor, int depth, double (*utility)(const Board& board, GameOutcome outcome, Color attackingcolor));

double MaterialisticUtility(const Board& board, GameOutcome outcome, Color attackingcolor);

struct MultiplierCompare {
  int multiplier_;
  MultiplierCompare(int multiplier) : multiplier_(multiplier) {}
  bool operator()(Move a, Move b) {
    return (multiplier_ * a.Utility()) < (multiplier_ * b.Utility());
  }
};

#endif