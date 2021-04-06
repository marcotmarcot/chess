#ifndef MOVE_H_
#define MOVE_H_

#include <optional>
#include <string>
#include <memory>

#include "position.h"

enum Promotion {
  kBishop,
  kKnight,
  kQueen,
  kRook,
};

class Move {
 public:
  static std::optional<Move> FromString(std::string from, std::string to);
  static std::optional<Move> FromXboardString(std::string move);

  Move(Position from, Position to, std::optional<Promotion> promotion);
  bool operator==(const Move& move) const;

  Position From() const;
  Position To() const;

  std::string String() const;
  std::string XboardString() const;

  void SetUtility(double utility);
  double Utility() const;

  std::optional<Promotion> PromoteTo() const;

 private:
  Position from_;
  Position to_;
  std::optional<Promotion> promotion_;
  double utility_;
};

#endif  // MOVE_H_
