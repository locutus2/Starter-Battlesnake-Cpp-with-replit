#include "helper.h"
#include "bot.h"

//----------------------------
// public API
//----------------------------

void Bot::setState(const json& data)
{
    auto& Y = data["you"];
    auto& B = data["board"];
    auto& G = data["game"];

    auto& S = B["snakes"];
    auto& R = G["ruleset"];

    turn = data["turn"];

    name = Y["name"];
    id = Y["id"];
    health = Y["health"];
    //latency = Y["latency"];

    timeout = G["timeout"];

    rule_name = R["name"];
    rule_version = R["version"];

    int height = B["height"];
    int width = B["width"];

    board.setDimensions(height, width);

    for(auto pos : B["food"])
        board.setSquare(pos["y"], pos["x"], { FOOD });

    for(auto pos : B["hazards"])
        board.setSquare(pos["y"], pos["x"], { HAZARD });


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
    cout << "----------------------------" << endl;
    cout << "turn: " << turn << endl;
    cout << "latency: " << latency << endl;
    cout << "length: " << snakes[snake_nr].body.size() << endl;

    Move move;
    //vector<Move> moves = board.generateSafeMoves(snakes[snake_nr].head);
    //vector<Move> moves = board.generateGreedySafeMoves(snakes[snake_nr].head);
    vector<Move> moves = board.generateLongLivingMoves(snakes[snake_nr].head, snakes[snake_nr].health);

    if(moves.empty())
        moves = board.generateSafeMoves(snakes[snake_nr].head);

    if(moves.empty())
        move = Move(rand() % 4);
    else
        move = moves[rand() % moves.size()];

    return move;
}

