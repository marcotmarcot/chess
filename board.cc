#include "board.h"

#include <iostream>
#include <vector>

#include "bishop.h"
#include "color.h"
#include "king.h"
#include "knight.h"
#include "move.h"
#include "pawn.h"
#include "piece.h"
#include "position.h"
#include "queen.h"
#include "rook.h"

namespace {

void SetUpRookKnightBishop(Color color, int others, int rook, int knight,
                           int bishop, std::unique_ptr<Piece> (&board)[8][8]) {
  board[rook][others] = std::make_unique<Rook>(color);
  board[knight][others] = std::make_unique<Knight>(color);
  board[bishop][others] = std::make_unique<Bishop>(color);
}

void SetUpColor(Color color, int others, int pawns,
                std::unique_ptr<Piece> (&board)[8][8]) {
  SetUpRookKnightBishop(color, others, 0, 1, 2, board);
  SetUpRookKnightBishop(color, others, 7, 6, 5, board);
  board[3][others] = std::make_unique<Queen>(color);
  board[4][others] = std::make_unique<King>(color);
  for (int x = 0; x <= 7; ++x) {
    board[x][pawns] = std::make_unique<Pawn>(color);
  }
}

}  // namespace

Board::Board() {
  SetUpColor(kWhite, 0, 1, board_);
  SetUpColor(kBlack, 7, 6, board_);
}

void Board::Print() const {
  for (int y = 7; y >= 0; --y) {
    std::cout << y + 1 << " ";
    for (int x = 0; x <= 7; ++x) {
      const Piece* piece = GetPiece({x, y});
      std::cout << (piece == nullptr ? " " : piece->String());
    }
    std::cout << std::endl;
  }
  std::cout << "  abcdefgh" << std::endl;
}

std::vector<Move> Board::GetMoves(Color color) const {
  std::vector<Move> moves;
  for (int x = 0; x <= 7; ++x) {
    for (int y = 0; y <= 7; ++y) {
      Position from(x, y);
      auto piece = GetPiece(from);
      if (!piece || piece->GetColor() != color) {
        continue;
      }
      std::vector<Position> tos = piece->GetMoves(*this, from);
      for (const auto to : tos) {
        moves.emplace_back(from, to);
      }
    }
  }
  return moves;
}

const Piece* Board::GetPiece(Position position) const {
  return board_[position.X()][position.Y()].get();
}
