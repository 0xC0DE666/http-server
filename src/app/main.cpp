#include <iostream>
#include <fstream>

#include "../lib/json.hpp"

#include "./server.h"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
  std::cout << "Http Server" << std::endl;

  Config* conf = load_config("/home/damian/dojo/cpp/http-server/src/assets/config.json");
  if (conf) {
    std::cout << conf->port << " " << conf->public_dir << std::endl;
  } else {
    std::cerr << "Failed to load config." << std::endl;
  }
  delete conf;

  return 0;
}
