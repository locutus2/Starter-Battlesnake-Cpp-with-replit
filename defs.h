#ifndef DEFS_H
#define DEFS_H

#include <vector>

using namespace std;

constexpr bool DEBUG = true;
constexpr bool VERBOSE = true;

constexpr int INF = 1000000;

constexpr int MAX_HEALTH = 100;
constexpr int MOVE_DAMAGE = 1;
constexpr int HAZARD_DAMAGE = 15;

constexpr int FOOD_GREEDINESS = INF;
//constexpr int FOOD_GREEDINESS = 0;

enum Move { NO_MOVE = -1, UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };
enum SQUARE { FREE = 0, FOOD = 1, HAZARD = 2, SNAKE = 3};

struct Coord
{
    int y, x;

    Coord(int y = -1, int x = -1) : y(y), x(x) {}

    bool operator<(const Coord& c)  const
    {
        return y < c.y || (y == c.y && x < c.x);
    }

    bool operator==(const Coord& c)  const
    {
        return y == c.y && x == c.x;
    }

    bool valid() const
    {
        return y >= 0 && x >= 0;
    }
};

struct Square
{
    SQUARE square = FREE;
    int snake = 0;
    int duration = 0;
};

struct Snake
{
    Coord head, tail;
    vector<Coord> body;
    int health;
    string id;

    Snake(const Coord& head, const vector<Coord>& body, int health, const string& id)
          : head(head), tail(body[body.size()-1]), body(body), health(health), id(id) {}
};

#endif
