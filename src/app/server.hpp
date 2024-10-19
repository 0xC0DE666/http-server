#include <iostream>
#include <string>

#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../lib/json.hpp"

#ifndef SERVER_H
#define SERVER_H

using std::string;
using nlohmann::json;

int add(int, int);

// ####################
// LOGGER
// ####################
enum LogLevel { ERROR, WARNING, INFO, DEBUG, VERBOSE };

class Logger {
public:
  LogLevel level;

  Logger();
  Logger(LogLevel lvl);

  ~Logger();

  void error(const string& msg);
  void warning(const string& msg);
  void info(const string& msg);
  void debug(const string& msg);
  void verbose(const string& msg);
};

// ####################
// CONFIG
// ####################
class Config {
public:
  int port;
  string public_dir; 

  Config();
  Config(string path);
  Config(int prt, string pub_dir);

  ~Config();

  string to_string();

private:
  Logger logger;

  void load(string path);
};

Config* load_config(string);

// ####################
// CONNECTION MANAGER
// ####################
#define BUFFER_SIZE 1024

class ConnectionManager {
public:
  ConnectionManager(Config* cfg);

  ~ConnectionManager();

  void init();
  void run();

private:
  Logger logger;
  Config* config;

  int server_fd;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
};

#endif
