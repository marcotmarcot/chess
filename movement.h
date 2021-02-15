#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <vector>

#include "board.h"
#include "color.h"
#include "position.h"

bool GetMove(const Board& board, Position from, Color color, int x, int y,
             std::vector<Position>& moves);

void GetLinearMoves(const Board& board, Position from, Color color,
                    int direction_x, int direction_y,
                    std::vector<Position>& moves);

#endif  // MOVEMENT_H_
