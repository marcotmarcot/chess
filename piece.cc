#include "piece.h"

#include <string>
#include <vector>

#include "board.h"
#include "color.h"
#include "move.h"
#include "position.h"

Piece::Piece(Color color) : color_(color) {}

Color Piece::GetColor() const { return color_; }

void Piece::NewTurn(Board& board, Position position) {}

void Piece::DoMove(Board& board, const Move& move) {}
