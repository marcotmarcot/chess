#include "king.h"

#include <string>
#include <vector>

#include "board.h"
#include "piece.h"
#include "movement.h"
#include "position.h"

std::string King::String() const { return GetColor() == kWhite ? "♔" : "♚"; }

std::vector<Position> King::GetMoves(const Board& board, Position from) const {
  std::vector<Position> moves;
  for (int x = -1; x <= 1; ++x) {
    for (int y = -1; y <= 1; ++y) {
      if (x == 0 && y == 0) {
        continue;
      }
      GetMove(board, from, GetColor(), x, y, moves);
    }
  }
  return moves;
}
