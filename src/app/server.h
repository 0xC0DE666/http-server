#ifndef SERVER_H
#define SERVER_H

#include <string>

using std::string;

int add(int, int);

// ####################
// CONFIG
// ####################
typedef struct Config {
  int port;
  string public_dir; 
} Config;

Config* load_config(string);

#endif
