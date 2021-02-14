#include "pawn.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "board.h"
#include "color.h"
#include "piece.h"
#include "position.h"

namespace {

int Forward(Color color) { return color == kWhite ? 1 : -1; }

void GetRegularMoves(const Board& board, Position from, Color color,
                  std::vector<Position>& moves) {
  auto next = from.Move(0, Forward(color));
  if (!next.has_value()) {
    std::cerr << "Pawn in the last row";
    exit(1);
  }
  if (board.GetPiece(*next)) {
    return;
  }
  moves.push_back(*next);
  auto two = next->Move(0, Forward(color));
  if (!two) {
    return;
  }
  if (board.GetPiece(*two)) {
    return;
  }
  moves.push_back(*two);
}

void GetCaptureMoves(const Board& board, Position from, Color color,
                  std::vector<Position>& moves) {
  for (int side = -1; side <= 1; side += 2) {
    auto to = from.Move(side, Forward(color));
    if (!to.has_value()) {
      continue;
    }
    auto toPiece = board.GetPiece(*to);
    if (toPiece == nullptr || toPiece->GetColor() == color) {
      continue;
    }
    moves.push_back(*to);
  }
}

void GetEnPassantMoves(const Board& board, Position from, Color color,
                    std::vector<Position>& moves) {
  for (int side = -1; side <= 1; side += 2) {
    auto onSide = from.Move(side, 0);
    if (!onSide) {
      continue;
    }
    auto piece = board.GetPiece(*onSide);
    if (!piece || piece->GetColor() == color) {
      continue;
    }
    auto pawn = dynamic_cast<const Pawn*>(piece);
    if (!pawn || !pawn->Double()) {
      continue;
    }
    auto to = from.Move(side, Forward(color));
    if (!to) {
      std::cerr << "Pawn in first row";
      exit(1);
    }
    moves.push_back(*to);
  }
}

}  // namespace

std::string Pawn::String() const { return GetColor() == kWhite ? "♙" : "♟"; }

std::vector<Position> Pawn::GetMoves(const Board& board, Position from) const {
  std::vector<Position> moves;
  GetRegularMoves(board, from, GetColor(), moves);
  GetCaptureMoves(board, from, GetColor(), moves);
  GetEnPassantMoves(board, from, GetColor(), moves);
  return moves;
}

bool Pawn::Double() const { return double_; }
