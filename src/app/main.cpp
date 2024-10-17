#include <iostream>
#include <fstream>

#include "../lib/json.hpp"

#include "./server.h"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
  std::cout << "Http Server" << std::endl;

  Config* conf = new Config("/home/damian/dojo/cpp/http-server/src/assets/config.json");
  ConnectionManager* con_man = new ConnectionManager(conf);

  delete con_man;
  delete conf;

  return 0;
}
