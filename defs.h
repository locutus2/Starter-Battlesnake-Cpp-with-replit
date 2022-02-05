#ifndef DEFS_H
#define DEFS_H

constexpr bool DEBUG = false;
constexpr bool VERBOSE = false;

constexpr int MAX_HEALTH = 100;
constexpr int MOVE_DAMAGE = 1;
constexpr int HAZARD_DAMAGE = 15;

//constexpr int FOOD_GREEDINESS = 1000000000;
constexpr int FOOD_GREEDINESS = 0;

enum Move { NO_MOVE = -1, UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };
enum SQUARE { FREE = 0, FOOD = 1, HAZARD = 2, SNAKE = 3};

struct Coord
{
    int y, x;

    Coord(int y = 0, int x = 0) : y(y), x(x) {}

    bool operator<(const Coord& c)  const
    {
        return y < c.y || (y == c.y && x < c.x);
    }

    bool operator==(const Coord& c)  const
    {
        return y == c.y && x == c.x;
    }
};

#endif
