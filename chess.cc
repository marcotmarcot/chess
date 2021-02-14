#include <iostream>

#include "board.h"
#include "color.h"

int main() {
  Board board;
  board.Print();
  for (const Move& move : board.GetMoves(kWhite)) {
    std::cout << move.String() << std::endl;
  }
  return 0;
}
