#include "knight.h"

#include <string>
#include <vector>

#include "board.h"
#include "movement.h"
#include "piece.h"
#include "position.h"

std::string Knight::String() const { return GetColor() == kWhite ? "♘" : "♞"; }

std::vector<Position> Knight::GetMoves(const Board& board,
                                       Position from) const {
  std::vector<Position> moves;
  for (int size_x = 1; size_x <= 2; ++size_x) {
    for (int direction_x = -1; direction_x <= 1; direction_x += 2) {
      for (int direction_y = -1; direction_y <= 1; direction_y += 2) {
        int size_y = 3 - size_x;
        int x = size_x * direction_x;
        int y = size_y * direction_y;
        GetMove(board, from, GetColor(), x, y, moves);
      }
    }
  }
  return moves;
}
