#ifndef BOT_H
#define BOT_H

#include "json.hpp"
#include "defs.h"
#include "board.h"

using namespace std;
using namespace nlohmann;

class Bot
{
    Board board;
    vector<Snake> snakes;
   
    // game 
    int turn;
    int timeout;
    string rule_name;
    string rule_version;

    // you
    int latency;
    int health;
    string id;
    string name;
    int snake_nr;

    public:
    void init(const json& data);
    void setState(const json& data);
    void start(const json& data);
    void end(const json& data);
    Move move();
};

#endif
