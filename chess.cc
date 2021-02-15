#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "board.h"
#include "color.h"
#include "move.h"

Move ReadHumanMove(const std::vector<Move>& valid_moves) {
  while (true) {
    std::cout << "Your move: ";
    std::string from, to;
    std::cin >> from >> to;
    auto move = Move::FromString(from, to);
    if (!move.has_value()) {
      std::cout << "Invalid syntax." << std::endl;
      continue;
    }
    if (std::find(valid_moves.begin(), valid_moves.end(), *move) == valid_moves.end()) {
      std::cout << "Invalid move. Valids: " << std::endl;
      for (const Move& valid_move : valid_moves) {
        std::cout << "  " << valid_move.String() << std::endl;
      }
      continue;
    }
    return *move;
  }
}

Move ChooseAiMove(const std::vector<Move>& valid_moves) {
  return valid_moves[rand() % valid_moves.size()];
}

int main() {
  srand(unsigned(time(nullptr)));
  Board board;
  while (true) {
    auto valid_human_moves = board.GetMoves(kWhite);
    if (valid_human_moves.empty()) {
      std::cout << "You lose!" << std::endl;
      return 0;
    }
    board.Print();
    Move human_move = ReadHumanMove(valid_human_moves);
    board.DoMove(human_move);
    board.Print();
    auto valid_ai_moves = board.GetMoves(kBlack);
    if (valid_ai_moves.empty()) {
      std::cout << "You win!" << std::endl;
      return 0;
    }
    Move ai_move = ChooseAiMove(valid_ai_moves);
    std::cout << "AI played: " << ai_move.String() << std::endl;
    board.DoMove(ai_move);
  }
  return 0;
}
