#include "board.h"

#include <algorithm>
#include <cstdlib>
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

std::vector<Move> GetMovesWithPossibleCheck(const Board& board, Color color) {
  std::vector<Move> moves;
  for (int x = 0; x <= 7; ++x) {
    for (int y = 0; y <= 7; ++y) {
      Position from(x, y);
      auto piece = board.GetPiece(from);
      if (!piece || piece->GetColor() != color) {
        continue;
      }
      std::vector<Position> tos = piece->GetMoves(board, from);
      for (const auto to : tos) {
        moves.emplace_back(from, to);
      }
    }
  }
  return moves;
}

std::unique_ptr<Piece>& GetMutablePiece(std::unique_ptr<Piece> (&board)[8][8], Position position) {
  return board[position.X()][position.Y()];
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
      std::cout << "\e[48;5;" << ((x + y) % 2 == 0 ? "208" : "94") << "m" << (piece == nullptr ? " " :  piece->String());
    }
    std::cout << "\e[0m";
    std::cout << std::endl;
  }
  std::cout << "  abcdefgh" << std::endl;
}

std::vector<Move> Board::GetMoves(Color color) {
  std::vector<Move> moves = GetMovesWithPossibleCheck(*this, color);
  for (auto i = moves.begin(); i != moves.end(); ) {
    std::unique_ptr<Piece> to = std::move(GetMutablePiece(board_, i->To()));
    DoMove(*i);
    bool is_check = IsCheck(color);
    DoMove(Move(i->To(), i->From()));
    GetMutablePiece(board_, i->To()) = std::move(to);
    if (is_check) {
      i = moves.erase(i);
      continue;
    }
    ++i;
  }
  return moves;
}

const Piece* Board::GetPiece(Position position) const {
  return board_[position.X()][position.Y()].get();
}

bool Board::IsCheck(Color color) const {
  std::vector<Move> moves = GetMovesWithPossibleCheck(*this, Other(color));
  return std::find_if(moves.begin(), moves.end(), [&](const Move& move) {
    auto piece = GetPiece(move.To());
    if (piece == nullptr) {
      return false;
    }
    if (piece->GetColor() != color) {
      std::cerr << "Moving to piece of the same color.";
      exit(1);
    }
    return dynamic_cast<const King*>(piece) != nullptr;
  }) != moves.end();
}

void Board::DoMove(const Move& move) {
  std::unique_ptr<Piece>& from = GetMutablePiece(board_, move.From());
  GetMutablePiece(board_, move.To()) = std::move(from);
  from = nullptr;
}
