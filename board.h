#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <tuple>

#include "defs.h"

using namespace std;

class Board
{
    int height = 0, width = 0;
    int maxSnakeLength = 0;

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
    vector<Move> generateRealLongLivingMoves2(const Snake& snake) const;
    vector<Move> generateRealLongLivingMoves3(const Snake& snake) const;
    vector<Move> generateRealLongLivingMoves4(const Snake& snake) const;
    tuple<int, Move, int> getShortestPathToSquareType(const Coord& head, SQUARE square) const;
    tuple<int, Move, int> getShortestPathTo(const Coord& source, const Coord& target, int health, bool checkAdjacent = false) const;
    tuple<int, Coord, int> getFarestFromSquares(const std::vector<Coord>& sources) const;
    tuple<int, Move, int> getLongestPathTo(const Coord& source, const Coord& target, int health, bool checkAdjacent = false) const;
    Move moveFromTo(const Coord& from, const Coord& to) const;
    Coord moveFrom(const Coord& from, Move move) const;
    void initMoveField(vector<vector<Move>> &moveField);
    int distanceTo(const Coord& pos, SQUARE squareType, bool allow_hazard = true) const;
    tuple<int, int, int, int> distanceToFoodAndSnake(const Coord& pos) const;
    bool adjacentTo(const Coord& a, const Coord& b) const;
    vector<Coord> getNeighbors(const Coord &pos) const;
};

#endif
