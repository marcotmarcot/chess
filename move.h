#ifndef MOVE_H_
#define MOVE_H_

#include <optional>
#include <string>
#include <memory>

#include "position.h"

class Move {
 public:
  static std::optional<Move> FromString(std::string from, std::string to);

  Move(Position from, Position to);
  bool operator==(const Move& move) const;

  Position From() const;
  Position To() const;

  std::string String() const;

  void SetUtility(double utility);
  double Utility() const;

 private:
  Position from_;
  Position to_;
  double utility_;
};

#endif  // MOVE_H_
