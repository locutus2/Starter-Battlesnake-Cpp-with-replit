//HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
//JSON credits: https://github.com/nlohmann/json
#include <iostream>
#include "./json.hpp"
#include "./http_stuff.h"
#include "./helper.h"
#include "./bot.h"

using namespace std;
using namespace nlohmann;

const char* HOST = "0.0.0.0";
constexpr int PORT = 8080;

const string HEAD = "sand-worm";
const string TAIL = "fat-rattle";
const string COLOR = "#996633";
const string AUTHOR = "locutus";

Bot bot;

int main(int argc, char* argv[]) {

  httplib::Server svr;

  svr.Get("/", [](const auto &, auto &res) {
    string head = HEAD;
    string tail = TAIL;
    string author = AUTHOR;
    string color = COLOR;
    res.set_content("{\"apiversion\":\"1\", \"head\":\"" + head + "\", \"tail\":\"" + tail + "\", \"color\":\"" + color + "\", " + "\"author\":\"" + author + "\"}", "application/json");
  });

  svr.Post("/end", [](const auto &, auto &res){
    bot.end();
    res.set_content("ok", "text/plain");
  });

  svr.Post("/start", [](const auto &, auto &res){
    bot.start();
    res.set_content("ok", "text/plain");
  });

  svr.Post("/move", [](auto &req, auto &res){
    const json data = json::parse(req.body);
    if(DEBUG && VERBOSE)
        cout << data << endl;

    bot.setState(data);
    
    string moves[4] = {"up", "down", "left", "right"};
    Move move = bot.move();
    cout << "=> " << moves[move] << endl << endl;
    res.set_content("{\"move\": \"" + moves[move] + "\"}", "text/plain");
  });

  int port = PORT;

  if(argc >= 2)
      port = stoi(argv[1]);

  cout << "Server started: " << HOST << ":" << port << endl;

  svr.listen(HOST, port);
}
