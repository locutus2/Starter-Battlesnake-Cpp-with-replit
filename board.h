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
    vector<vector<Move>> moveField;

    public:
    void setDimensions(int height, int width);
    void setSquare(int y, int x, const Square& square);
    Square getSquare(int y, int x) const;
    vector<Move> generateAllSafeMoves(const Coord& head, bool onlyFood = false, bool noFood = false) const;
    vector<Move> generateSafeMoves(const Coord& head, bool onlyFood = false, bool noFood = false) const;
    vector<Move> generateGreedySafeMoves(const Coord& head) const;
    vector<Move> generateLongLivingMoves_old(const Coord& head, int health) const;
    vector<Move> generateLongLivingMoves(const Coord& head, int health) const;
    vector<Move> generateRealLongLivingMoves(const Coord& head, int health, int lenght) const;
    Move getShortestPathToSquareType(const Coord& head, SQUARE square) const;
    Move moveFromTo(const Coord& from, const Coord& to) const;
    Coord moveFrom(const Coord& from, Move move) const;
    void initMoveField(vector<vector<Move>> &moveField);
    int distanceTo(const Coord& pos, SQUARE squareType, bool allow_hazard = true) const;
};

#endif
