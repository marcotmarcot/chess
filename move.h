#ifndef MOVE_H_
#define MOVE_H_

#include <optional>
#include <string>

#include "position.h"

class Move {
 public:
  static std::optional<Move> FromString(std::string from, std::string to);

  Move(Position from, Position to);

  std::string String() const;

 private:
  Position from_;
  Position to_;
};

#endif  // MOVE_H_