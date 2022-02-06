//HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
//JSON credits: https://github.com/nlohmann/json
#include <iostream>
#include <map>
#include "./json.hpp"
#include "./http_stuff.h"
#include "./helper.h"
#include "./bot.h"

using namespace std;
using namespace nlohmann;

const char* HOST = "0.0.0.0";
constexpr int PORT = 8080;

const string API_VERSION = "1";
const string HEAD = "sand-worm";
const string TAIL = "fat-rattle";
const string COLOR = "#996633";
const string AUTHOR = "locutus";

map<string, Bot*> games;

void startGame(const json& data)
{
    string game_id = data["game"]["id"];
    if (games.find(game_id) != games.end())
    {
        delete games[game_id];
        games.erase(game_id);
    }

    games[game_id] = new Bot();
    games[game_id]->start(data);
}

void endGame(const json& data)
{
    string game_id = data["game"]["id"];
    if (games.find(game_id) != games.end())
    {
        games[game_id]->end(data);
        delete games[game_id];
        games.erase(game_id);
    }
}

Bot* getGame(const json& data)
{
    string game_id = data["game"]["id"];
    if (games.find(game_id) == games.end())
        startGame(data);

    return games[game_id];
}

int main(int argc, char* argv[]) {

  httplib::Server svr;

  svr.Get("/", [](const auto &req, auto &res) {
    string api_version = API_VERSION;
    string head = HEAD;
    string tail = TAIL;
    string author = AUTHOR;
    string color = COLOR;
    res.set_content("{\"apiversion\":\"" + api_version+ "\", \"head\":\"" + head + "\", \"tail\":\"" + tail + "\", \"color\":\"" + color + "\", " + "\"author\":\"" + author + "\"}", "application/json");
  });

  svr.Post("/end", [](const auto &req, auto &res){
    const json data = json::parse(req.body);
    endGame(data);
    res.set_content("ok", "text/plain");
  });

  svr.Post("/start", [](const auto &req, auto &res){
    const json data = json::parse(req.body);
    startGame(data);
    res.set_content("ok", "text/plain");
  });

  svr.Post("/move", [](auto &req, auto &res){
    const json data = json::parse(req.body);
    if(DEBUG && VERBOSE)
        cout << data << endl;

    Bot* bot = getGame(data);
    bot->setState(data);
    
    string moves[4] = {"up", "down", "left", "right"};
    Move move = bot->move();
    cout << "=> " << moves[move] << endl << endl;
    res.set_content("{\"move\": \"" + moves[move] + "\"}", "text/plain");
  });

  int port = PORT;

  if(argc >= 2)
      port = stoi(argv[1]);

  cout << "Server started: " << HOST << ":" << port << endl;

  svr.listen(HOST, port);
}
