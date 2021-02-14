#ifndef KNIGHT_H_
#define KNIGHT_H_

#include <string>
#include <vector>

#include "board.h"
#include "piece.h"
#include "position.h"

class Knight : public Piece {
 public:
  using Piece::Piece;

  std::string String() const override;

  std::vector<Position> GetMoves(const Board& board, Position from) const override;
};

#endif  // KNIGHT_H_
