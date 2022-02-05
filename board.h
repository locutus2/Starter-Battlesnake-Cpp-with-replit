#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "defs.h"

using namespace std;

struct Square
{
    SQUARE square = FREE;
    int snake = 0;
};

class Board
{
    int height = 0, width = 0;

    vector<vector<Square>> board;

    public:
    void setDimensions(int height, int width);
    void setSquare(int y, int x, const Square& square);
    Square getSquare(int y, int x) const;
    vector<Move> generateSafeMoves(const Coord& head, bool onlyFood = false) const;
    vector<Move> generateGreedySafeMoves(const Coord& head) const;
};

#endif
