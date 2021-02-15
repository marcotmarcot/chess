#ifndef PIECE_H_
#define PIECE_H_

#include <string>
#include <vector>

#include "color.h"
#include "move.h"
#include "position.h"

class Board;

class Piece {
 public:
  Piece(Color color);

  virtual ~Piece() = default;

  virtual std::string String() const = 0;

  virtual std::vector<Position> GetMoves(const Board& board,
                                         Position from) const = 0;

  Color GetColor() const;

  virtual void NewTurn(Board& board, Position position);

  virtual void DoMove(Board& board, const Move& move);

 private:
  const Color color_;
};

#endif  // PIECE_H_
