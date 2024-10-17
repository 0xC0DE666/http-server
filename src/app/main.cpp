#include <iostream>

#include "../lib/json.hpp"

#include "./util.h"

int main(int argc, char* argv[]) {
  std::cout << "Http Server" << std::endl;
  std::cout << add(argc, 100) << std::endl;
  for (int i = 0; i < argc; i++) {
    std::cout << argv[i] << std::endl;
  }

  // Create a JSON object
  nlohmann::json j;
  j["name"] = "Arch Linux";
  j["version"] = "2024.10";
  
  // Print the JSON object
  std::cout << j.dump(4) << std::endl;

  
  return 0;
}
