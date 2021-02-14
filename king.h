#ifndef KING_H_
#define KING_H_

#include <string>
#include <vector>

#include "board.h"
#include "piece.h"
#include "position.h"

class King : public Piece {
 public:
  using Piece::Piece;

  std::string String() const override;

  std::vector<Position> GetMoves(const Board& board, Position from) const override;

 private:
  bool moved_;
};

#endif  // KING_H_
