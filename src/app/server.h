#include <iostream>
#include <string>

#ifndef SERVER_H
#define SERVER_H

using std::string;

int add(int, int);

// ####################
// CONFIG
// ####################
class Config {
  public:
  int port;
  string public_dir; 

  Config(int prt, string pub_dir) {
    port = prt;
    public_dir = pub_dir;
  }

  string to_string() {
    return std::to_string(port) + " " + public_dir;
  }
};

Config* load_config(string);

// ####################
// CONNECTION MANAGER
// ####################
class ConnectionManager {
  private:
  Config* config;

  public:
  ConnectionManager(Config* cfg) {
    config = cfg;
    std::cout << config->to_string() << std::endl;
  }

  ~ConnectionManager() {}
};


#endif
