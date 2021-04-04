#ifndef QUEEN_H_
#define QUEEN_H_

#include <string>
#include <vector>

#include "board.h"
#include "piece.h"
#include "position.h"

class Queen : public Piece {
 public:
  using Piece::Piece;

  std::string String() const override;

  std::vector<Position> GetMoves(const Board& board,
                                 Position from) const override;

  int Value() const override;

  Piece* Clone() const override;
};

#endif  // QUEEN_H_
