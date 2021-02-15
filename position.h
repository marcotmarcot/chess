#ifndef POSITION_H_
#define POSITION_H_

#include <optional>
#include <string>

class Position {
 public:
  static std::optional<Position> FromString(std::string s);
  Position(int x, int y);
  bool operator==(const Position& position) const;
  int X() const;
  int Y() const;
  std::optional<Position> Move(int dx, int dy) const;
  std::string String() const;

 private:
  int x_;
  int y_;
};

#endif  // POSITION_H_
