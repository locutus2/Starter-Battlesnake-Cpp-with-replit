//HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
//JSON credits: https://github.com/nlohmann/json
#include <iostream>
#include "./json.hpp"
#include "./http_stuff.h"
#include "./bot.h"

using namespace std;
using namespace nlohmann;

constexpr int PORT = 8082;

Bot bot;

int main(void) {

  httplib::Server svr;

  svr.Get("/", [](const auto &, auto &res) {
    string head = "default"; //TODO: Change head
    string tail = "default"; //TODO: Change tail
    string author = "locutus"; //TODO: Change your battlesnake username
    string color = "#008800";  //TODO: Change a hex color
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
    cout << data;
    cout << endl;

    bot.setState(data);

    string moves[4] = {"up", "down", "left", "right"};
    Move move = bot.move();
    cout << "=> " << moves[move] << endl << endl;
    res.set_content("{\"move\": \"" + moves[move] + "\"}", "text/plain");
  });

  svr.listen("0.0.0.0", PORT);
  cout << "Server started";
}
