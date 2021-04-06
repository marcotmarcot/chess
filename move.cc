#include "move.h"

#include <iostream>
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
  return Move(*from_position, *to_position, std::nullopt);
}

std::optional<Move> Move::FromXboardString(std::string move) {
  if (move.size() < 4) {
    return {};
  }
  auto from_position = Position::FromString(move.substr(0, 2));
  if (!from_position.has_value()) {
    return {};
  }
  auto to_position = Position::FromString(move.substr(2, 2));
  if (!to_position.has_value()) {
    return {};
  }
  std::optional<Promotion> promotion;
  if (move.size() > 4) {
    std::string prom = move.substr(4, 1);
    if (prom == "b") {
      promotion = kBishop;
    } else if (prom == "k") {
      promotion = kKnight;
    } else if (prom == "q") {
      promotion = kQueen;
    } else if (prom == "r") {
      promotion = kKnight;
    }
  }
  return Move(*from_position, *to_position, promotion);
}

Move::Move(Position from, Position to, std::optional<Promotion> promotion) : from_(from), to_(to), promotion_(promotion), utility_(0.0) {}

bool Move::operator==(const Move& move) const {
  return From() == move.From() && To() == move.To();
}

Position Move::From() const { return from_; }

Position Move::To() const { return to_; }

std::string Move::String() const { return from_.String() + " " + to_.String(); }

std::string Move::XboardString() const { return from_.String() + to_.String(); }

void Move::SetUtility(double utility) { utility_ = utility; }

double Move::Utility() const { return utility_; }

std::optional<Promotion> Move::PromoteTo() const { return promotion_; };