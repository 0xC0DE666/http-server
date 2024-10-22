#include <iostream>
#include <fstream>

#include "../lib/json.hpp"

#include "./server.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
  std::cout << "Http Server" << std::endl;

  Config* conf = Config::load("/home/damian/dojo/cpp/http-server/src/assets/config.json");
  ClientManager* cli_man = new ClientManager(conf);

  cli_man->init();
  cli_man->run();

  delete cli_man;
  delete conf;

  return 0;
}
