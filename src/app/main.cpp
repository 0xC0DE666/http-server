#include <iostream>

#include "./util.h"

int main(int argc, char* argv[]) {
  std::cout << "Http Server" << std::endl;
  std::cout << add(argc, 100) << std::endl;
  for (int i = 0; i < argc; i++) {
    std::cout << argv[i] << std::endl;
  }
  
  return 0;
}
