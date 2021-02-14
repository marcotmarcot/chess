#ifndef POSITION_H_
#define POSITION_H_

#include <optional>
#include <string>

class Position {
 public:
  static std::optional<Position> FromString(std::string s);
  Position(int x, int y);
  int X() const;
  int Y() const;
  std::optional<Position> Move(int dx, int dy) const;
  std::string String() const;

 private:
  const int x_;
  const int y_;
};

#endif  // POSITION_H_
