#ifndef ROOK_H_
#define ROOK_H_

#include <string>
#include <vector>

#include "board.h"
#include "color.h"
#include "move.h"
#include "piece.h"
#include "position.h"

class Rook : public Piece {
 public:
  using Piece::Piece;

  std::string String() const override;

  std::vector<Position> GetMoves(const Board& board,
                                 Position from) const override;

  void DoMove(Board& board, const Move& move) override;

  bool Moved() const;

  int Value() const override;

  Piece* Clone() const override;

 private:
  bool moved_;
};

void GetRookMoves(const Board& board, Position from, Color color,
                  std::vector<Position>& moves);

#endif  // ROOK_H_
