#include "queen.h"

#include <string>
#include <vector>

#include "bishop.h"
#include "board.h"
#include "piece.h"
#include "position.h"
#include "rook.h"

std::string Queen::String() const { return GetColor() == kWhite ? "♕" : "♛"; }

std::vector<Position> Queen::GetMoves(const Board& board, Position from) const {
  std::vector<Position> moves;
  GetBishopMoves(board, from, GetColor(), moves);
  GetRookMoves(board, from, GetColor(), moves);
  return moves;
}
