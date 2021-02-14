#include "piece.h"

#include <string>
#include <vector>

#include "board.h"
#include "color.h"
#include "position.h"

Piece::Piece(Color color) : color_(color) {}

Color Piece::GetColor() const { return color_; }
