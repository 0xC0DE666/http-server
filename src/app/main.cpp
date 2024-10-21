#include <iostream>
#include <fstream>

#include "../lib/json.hpp"

#include "./server.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
  std::string get = Http::Method::GET;
  std::cout << get << std::endl;

  Http::Status not_found = Http::Status::NOT_FOUND;
  std::cout << std::to_string(not_found.CODE) << " " << not_found.MESSAGE << std::endl;

  std::cout << "Http Server" << std::endl;

  Config* conf = new Config("/home/damian/dojo/cpp/http-server/src/assets/config.json");
  ClientManager* con_man = new ClientManager(conf);

  con_man->init();
  con_man->run();

  delete con_man;
  delete conf;

  return 0;
}
