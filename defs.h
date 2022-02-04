#ifndef DEFS_H
#define DEFS_H

enum Move { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };
enum SQUARE { FREE = 0, FOOD = 1, HAZARD = 2, SNAKE = 3};

struct Coord
{
    int y, x;

    Coord(int y = 0, int x = 0) : y(y), x(x) {}
    bool operator<(const Coord& c) 
    {
        return y < c.y || (y == c.y && x < c.x);
    }
};

#endif
