#include "move.h"

#include <optional>
#include <string>
#include <memory>

#include "position.h"

std::optional<Move> Move::FromString(std::string from, std::string to) {
  auto from_position = Position::FromString(from);
  if (!from_position.has_value()) {
    return {};
  }
  auto to_position = Position::FromString(to);
  if (!to_position.has_value()) {
    return {};
  }
  return Move(*from_position, *to_position);
}

Move::Move(Position from, Position to) : from_(from), to_(to), utility_(0.0) {}

bool Move::operator==(const Move& move) const {
  return From() == move.From() && To() == move.To();
}

Position Move::From() const { return from_; }

Position Move::To() const { return to_; }

std::string Move::String() const { return from_.String() + " " + to_.String(); }

void Move::SetUtility(double utility) { utility_ = utility; }

double Move::Utility() const { return utility_; }
