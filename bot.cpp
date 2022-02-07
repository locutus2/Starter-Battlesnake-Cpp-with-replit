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

    if(rule_name == "standard")
        gameMode = STANDARD;
    else if(rule_name == "solo")
        gameMode = SOLO;
    else if(rule_name == "royale")
        gameMode = ROYALE;
    else if(rule_name == "squad")
        gameMode = SQUAD;
    else if(rule_name == "constrictor")
        gameMode = CONSTRICTOR;
    else if(rule_name == "wrapped")
        gameMode = WRAPPED;
    else
        gameMode = DEFAULT_GAME_MODE;

    int height = B["height"];
    int width = B["width"];

    board.setDimensions(height, width);
    board.setGameMode(gameMode);

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

	int len = S[s]["length"];
        for(auto pos : S[s]["body"])
        {
            board.setSquare(pos["y"], pos["x"], {SNAKE, s, len});
            snakeBody.push_back({pos["y"], pos["x"]});
	    len--;
        }

        snakes.push_back(Snake(snakeHead, snakeBody, health, snake_id));
    }
    
}

void Bot::init(const json& data)
{
    board.setDimensions(-1, -1);
}

void Bot::start(const json& data)
{
    string game_id = data["game"]["id"];
    string bot_id = data["you"]["id"];
    cout << "======= Start game: game_id=" << game_id << " bot_id=" << bot_id << " ================" << endl;

    init(data);
}

void Bot::end(const json& data)
{
    string game_id = data["game"]["id"];
    string bot_id = data["you"]["id"];
    cout << "======= End game: game_id=" << game_id << " bot_id=" << bot_id << " ================" << endl;

    //int health = data["you"]["health"];
    //int length = data["you"]["length"];
    //cout << "health: " << health << endl;
    //cout << "length: " << length << endl;
}

Move Bot::move()
{
    cout << "----------------------------" << endl;
    cout << "turn: " << turn << endl;
    cout << "latency: " << latency << endl;
    cout << "health: " << health << endl;
    cout << "length: " << snakes[snake_nr].body.size() << endl;

    Move move;
    //vector<Move> moves = board.generateSafeMoves(snakes[snake_nr].head);
    //vector<Move> moves = board.generateGreedySafeMoves(snakes[snake_nr].head);
    //vector<Move> moves = board.generateLongLivingMoves(snakes[snake_nr].head, snakes[snake_nr].health);
    //vector<Move> moves = board.generateRealLongLivingMoves(snakes[snake_nr].head, snakes[snake_nr].health, snakes[snake_nr].body.size());
    //vector<Move> moves = board.generateRealLongLivingMoves2(snakes[snake_nr]);
    //vector<Move> moves = board.generateRealLongLivingMoves3(snakes[snake_nr]);
    vector<Move> moves = board.generateRealLongLivingMoves4(snakes[snake_nr]);

    if(moves.empty())
        moves = board.generateSafeMoves(snakes[snake_nr].head);

    if(moves.empty())
        move = Move(rand() % 4);
    else
        move = moves[rand() % moves.size()];

    return move;
}

