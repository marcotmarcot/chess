#include "movement.h"

#include <vector>

#include "board.h"
#include "color.h"
#include "position.h"

bool GetMove(const Board& board, Position from, Color color, int x, int y,
             std::vector<Position>& moves) {
  auto to = from.Move(x, y);
  if (!to.has_value()) {
    return false;
  }
  auto piece = board.GetPiece(*to);
  if (piece == nullptr) {
    moves.push_back(*to);
    return true;
  }
  if (piece->GetColor() != color) {
    moves.push_back(*to);
  }
  return false;
}

void GetLinearMoves(const Board& board, Position from, Color color,
                    int direction_x, int direction_y,
                    std::vector<Position>& moves) {
  for (int size = 1; true; ++size) {
    int x = size * direction_x;
    int y = size * direction_y;
    if (!GetMove(board, from, color, x, y, moves)) {
      return;
    }
  }
}
