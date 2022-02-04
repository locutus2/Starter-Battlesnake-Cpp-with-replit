#include "board.h"

void Board::setDimensions(int height, int width)
{
    if(height != this->height || width != this->width)
    {
        this->height = height;
        this->width = width;
    }
    board = vector<vector<Square>>(height, vector<Square>(width));
}

void Board::setSquare(int y, int x, const Square& square)
{
    board[y][x] = square;
}

Square Board::getSquare(int y, int x) const
{
    return board[y][x];
}

vector<Move> Board::generateSafeMoves(const Coord& head) const
{
    vector<Move> moves;

    if (head.y > 0 && (   getSquare(head.y-1, head.x).square == FREE
                       || getSquare(head.y-1, head.x).square == FOOD))
        moves.push_back(DOWN);

    if (head.y < height-1 && (   getSquare(head.y+1, head.x).square == FREE
                              || getSquare(head.y+1, head.x).square == FOOD))
        moves.push_back(UP);

    if (head.x > 0 && (   getSquare(head.y, head.x-1).square == FREE
                       || getSquare(head.y, head.x-1).square == FOOD))
        moves.push_back(LEFT);

    if (head.x < width-1 && (   getSquare(head.y, head.x+1).square == FREE
                             || getSquare(head.y, head.x+1).square == FOOD))
        moves.push_back(RIGHT);

    return moves;
}

