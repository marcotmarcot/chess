#include "position.h"

#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

namespace {
bool Valid(int v) { return v >= 0 && v <= 7; }
}  // namespace

std::optional<Position> Position::FromString(std::string s) {
  if (s.size() != 2) {
    return {};
  }
  int x = s[0] - 'a';
  int y = s[1] - '1';
  if (!Valid(x) || !Valid(y)) {
    return {};
  }
  return Position(x, y);
}

Position::Position(int x, int y) : x_(x), y_(y) {
  if (!Valid(x) || !Valid(y)) {
    std::cerr << "Invalid position";
    exit(1);
  }
}

bool Position::operator==(const Position& position) const {
  return X() == position.X() && Y() == position.Y();
}

int Position::X() const { return x_; }

int Position::Y() const { return y_; }

std::optional<Position> Position::Move(int dx, int dy) const {
  int nx = x_ + dx;
  int ny = y_ + dy;
  if (Valid(nx) && Valid(ny)) {
    return Position(nx, ny);
  }
  return {};
}

std::string Position::String() const {
  std::string print;
  print += 'a' + x_;
  print += std::to_string(y_ + 1);
  return print;
}
