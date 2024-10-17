#include <iostream>
#include <fstream>
#include <string>

#include "../lib/json.hpp"
#include "./server.h"

using std::string;
using nlohmann::json;

int add(int a, int b) {
  return a + b;
}

Config* load_config(string path) {
  std::cerr << "Loading config." << std::endl;
  Config* conf = nullptr;

  try {
    std::ifstream file(path);

    if (!file.is_open()) {
       std::cerr << "Failed to open file!" << std::endl;
       return nullptr;
    }

    json data = json::parse(file);
    // std::cout << data.dump(4) << std::endl;

    conf = new Config(data["port"], data["public_dir"]);
  } catch (const std::runtime_error& e) {
    std::cerr << "Failed to load config: " << e.what() << std::endl;
  }

  return conf;
}
