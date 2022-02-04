#include "bot.h"

//----------------------------
// public API
//----------------------------

void Bot::setState(const json& data)
{
    auto& Y = data["you"];
    auto& B = data["board"];

    name = Y["name"];
    id = Y["id"];

    int height = B["height"];
    int width = B["width"];

    board.setDimensions(height, width);

    for(auto pos : B["food"])
        board.setSquare(pos["y"], pos["x"], { FOOD });

    for(auto pos : B["hazards"])
        board.setSquare(pos["y"], pos["x"], { HAZARD });

    auto& S = B["snakes"];

    snakes.clear();
    for(int s = 0; s < (int)S.size(); ++s)
    {
        std::vector<Coord> snakeBody;
        Coord snakeHead = { S[s]["head"]["y"], S[s]["head"]["x"] };
        int health = S[s]["health"];
        string snake_id = S[s]["id"];

        if(id == snake_id)
            snake_nr = s;

        for(auto pos : S[s]["body"])
        {
            board.setSquare(pos["y"], pos["x"], {SNAKE, s});
            snakeBody.push_back({pos["y"], pos["x"]});
        }

        snakes.push_back(Snake(snakeHead, snakeBody, health, snake_id));
    }
    
}

void Bot::start()
{
}

void Bot::end()
{
}

Move Bot::move()
{
    Move move;
    vector<Move> moves = board.generateSafeMoves(snakes[snake_nr].head);

    if(moves.empty())
        move = Move(rand() % 4);
    else
        move = moves[rand() % moves.size()];

    return move;
}

