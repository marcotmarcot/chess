#include "bishop.h"

#include <string>
#include <vector>

#include "board.h"
#include "color.h"
#include "movement.h"
#include "piece.h"
#include "position.h"

std::string Bishop::String() const { return GetColor() == kWhite ? "♗" : "♝"; }

std::vector<Position> Bishop::GetMoves(const Board& board, Position from) const {
  std::vector<Position> moves;
  GetBishopMoves(board, from, GetColor(), moves);
  return moves;
}

void GetBishopMoves(const Board& board, Position from, Color color,
                 std::vector<Position>& moves) {
  for (int direction_x = -1; direction_x <= 1; direction_x += 2) {
    for (int direction_y = -1; direction_y <= 1; direction_y += 2) {
      GetLinearMoves(board, from, color, direction_x, direction_y, moves);
    }
  }
}
