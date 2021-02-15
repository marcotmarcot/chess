#ifndef BISHOP_H_
#define BISHOP_H_

#include <string>
#include <vector>

#include "board.h"
#include "color.h"
#include "piece.h"
#include "position.h"

class Bishop : public Piece {
 public:
  using Piece::Piece;

  std::string String() const override;

  std::vector<Position> GetMoves(const Board& board, Position from) const override;
};

void GetBishopMoves(const Board& board, Position from, Color color,
                 std::vector<Position>& moves);

#endif  // BISHOP_H_
