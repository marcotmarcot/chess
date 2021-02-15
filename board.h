#ifndef BOARD_H_
#define BOARD_H_

#include <memory>
#include <vector>

#include "color.h"
#include "move.h"
#include "piece.h"
#include "position.h"

class Board {
 public:
  Board();
  void Print() const;
  std::vector<Move> GetMoves(Color color);
  const Piece* GetPiece(Position position) const;
  bool IsCheck(Color color) const;
  void DoMove(const Move& move);

 private:
  std::unique_ptr<Piece> board_[8][8];
};

#endif  // BOARD_H_
