#include "board.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <list>

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
      bool is_pawn = dynamic_cast<const Pawn*>(piece) != nullptr;
      for (const auto to : tos) {
        if (is_pawn && (to.Y() == 0 || to.Y() == 7)) {
          moves.emplace_back(from, to, kBishop);
          moves.emplace_back(from, to, kKnight);
          moves.emplace_back(from, to, kQueen);
          moves.emplace_back(from, to, kRook);
        } else {
          moves.emplace_back(from, to, std::nullopt);
        }
      }
    }
  }
  return moves;
}

std::unique_ptr<Piece>& GetMutablePiece(std::unique_ptr<Piece> (&board)[8][8],
                                        Position position) {
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
      std::cout << "\e[48;5;" << ((x + y) % 2 == 0 ? "94" : "208") << "m"
                << (piece == nullptr ? " " : piece->String());
    }
    std::cout << "\e[0m";
    std::cout << std::endl;
  }
  std::cout << "  abcdefgh" << std::endl;
}

std::vector<Move> Board::GetMoves(Color color) {
  std::vector<Move> moves = GetMovesWithPossibleCheck(*this, color);
  for (auto i = moves.begin(); i != moves.end();) {
    std::unique_ptr<Piece>& from = GetMutablePiece(board_, i->From());
    std::unique_ptr<Piece>& to = GetMutablePiece(board_, i->To());
    std::unique_ptr<Piece> to_copy = std::move(to);
    to = std::move(from);
    bool is_check = IsCheck(color);
    from = std::move(to);
    to = std::move(to_copy);
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

std::optional<Position> Board::FindKing(Color color) const {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j <  8; ++j) {
      const King* king = dynamic_cast<const King*>(board_[i][j].get());
      if (king && king->GetColor() == color) {
        return Position(i, j);
      }
    }
  }
  return {};
}

bool Board::IsCheck(Color color) const {
  auto kingpos = FindKing(color);
  if (!kingpos.has_value()) {
    return false;
  }
  Bishop bishop(color);
  for (auto pos : bishop.GetMoves(*this, kingpos.value())) {
    const Piece *p = this->GetPiece(pos);
    if (dynamic_cast<const Bishop*>(p) != nullptr) {
      return true;
    } else if (dynamic_cast<const Queen*>(p) != nullptr) {
      return true;
    }
  }
  Rook rook(color);
  for (auto pos : rook.GetMoves(*this, kingpos.value())) {
    const Piece *p = this->GetPiece(pos);
    if (dynamic_cast<const Rook*>(p) != nullptr) {
      return true;
    } else if (dynamic_cast<const Queen*>(p) != nullptr) {
      return true;
    }
  }
  Knight knight(color);
  for (auto pos : knight.GetMoves(*this, kingpos.value())) {
    const Piece *p = this->GetPiece(pos);
    if (dynamic_cast<const Knight*>(p) != nullptr) {
      return true;
    }
  }
  std::vector<Position> pawns;
  if (color == kWhite) {
    int x, y;
    x = kingpos.value().X() + 1;
    y = kingpos.value().Y() + 1;
    if (x < 8 && y < 8) {
      pawns.emplace_back(x, y);
    }
    x = kingpos.value().X() - 1;
    y = kingpos.value().Y() + 1;
    if (x >= 0 && y < 8) {
      pawns.emplace_back(x, y);
    }
  } else {
    int x, y;
    x = kingpos.value().X() + 1;
    y = kingpos.value().Y() - 1;
    if (x < 8 && y >= 0) {
      pawns.emplace_back(x, y);
    }
    x = kingpos.value().X() - 1;
    y = kingpos.value().Y() - 1;
    if (x >= 0 && y >= 0) {
      pawns.emplace_back(x, y);
    }
  }
  for (auto pos : pawns) {
    const Piece *p = this->GetPiece(pos);
    if (dynamic_cast<const Pawn*>(p) != nullptr && p->GetColor() == Other(color)) {
      return true;
    }
  }
  return false;
}

void Board::DoMove(const Move& move) {
  std::unique_ptr<Piece>& from = GetMutablePiece(board_, move.From());
  std::unique_ptr<Piece>& to = GetMutablePiece(board_, move.To());
  to = std::move(from);
  from = nullptr;
  to->DoMove(*this, move);
}

void Board::NewTurn(Color color) {
  for (int x = 0; x <= 7; ++x) {
    for (int y = 0; y <= 7; ++y) {
      Piece* piece = board_[x][y].get();
      if (piece != nullptr && piece->GetColor() == color) {
        piece->NewTurn();
      }
    }
  }
}

void Board::Set(Position position, std::unique_ptr<Piece> piece) {
  GetMutablePiece(board_, position) = std::move(piece);
}

std::list<const Piece*> Board::GetPieces() const {
  std::list<const Piece*> pieces;
  for (int x = 0; x <= 7; ++x) {
    for (int y = 0; y <= 7; ++y) {
      Piece* piece = board_[x][y].get();
      if (piece != nullptr) {
        pieces.push_back(piece);
      }
    }
  }
  return pieces;
}

std::unique_ptr<Board::Snapshot> Board::TakeSnapshot() const {
  Piece* board[8][8];
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (board_[i][j] != nullptr) {
        board[i][j] = board_[i][j]->Clone();
      } else {
        board[i][j] = nullptr;
      }
    }
  }
  return std::unique_ptr<Board::Snapshot>(new Board::Snapshot(board));
}

void Board::RecoverSnapshot(const Board::Snapshot& snapshot) {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (snapshot.board_[i][j] != nullptr) {
        board_[i][j] = std::unique_ptr<Piece>(snapshot.board_[i][j]->Clone());
      } else {
        board_[i][j].reset();
      }
    }
  }
}

Board::Snapshot::Snapshot(Piece* board[8][8]) {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      board_[i][j] = std::unique_ptr<Piece>(board[i][j]);
    }
  }
}