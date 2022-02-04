#ifndef BOT_H
#define BOT_H

#include "json.hpp"
#include "defs.h"
#include "board.h"

using namespace std;
using namespace nlohmann;

struct Snake
{
    Coord head;
    vector<Coord> body;
    int health;
    string id;

    Snake(const Coord& head, const vector<Coord>& body, int health, const string& id)
	    : head(head), body(body), health(health), id(id) {}
};

class Bot
{
    Board board;
    std::string id;
    std::string name;
    vector<Snake> snakes;
    int snake_nr;

    public:
    void setState(const json& data);
    void start();
    void end();
    Move move();
};

#endif
