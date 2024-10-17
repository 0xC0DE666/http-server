#include <iostream>
#include <string>

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

  Logger() {
    level = LogLevel::INFO;
  }

  Logger(LogLevel lvl) {
    level = lvl;
  }

  ~Logger() {}

  void error(const string& msg) {
    if (level < LogLevel::ERROR) return;
    std::cerr << msg << std::endl;
  }

  void warning(const string& msg) {
    if (level < LogLevel::WARNING) return;
    std::cout << msg << std::endl;
  }

  void info(const string& msg) {
    if (level < LogLevel::INFO) return;
    std::cout << msg << std::endl;
  }

  void debug(const string& msg) {
    if (level < LogLevel::DEBUG) return;
    std::cout << msg << std::endl;
  }

  void verbose(const string& msg) {
    if (level < LogLevel::VERBOSE) return;
    std::cout << msg << std::endl;
  }
};

// ####################
// CONFIG
// ####################
class Config {
public:
  int port;
  string public_dir; 

  Config() {
    logger = Logger();
  }

  Config(string path) {
    load(path);
    logger = Logger();
  }

  Config(int prt, string pub_dir) {
    port = prt;
    public_dir = pub_dir;
    logger = Logger();
  }

  ~Config() {}

  string to_string() {
    return std::to_string(port) + " " + public_dir;
  }

private:
  Logger logger;

  void load(string path) {
    try {
      std::ifstream file(path);
  
      if (!file.is_open()) {
        logger.error("Failed to open config file!");
      }
  
      json data = json::parse(file);
  
      port = data["port"];
      public_dir = data["public_dir"];

      logger.info("Loaded config.");
    } catch (const std::runtime_error& e) {
      logger.error("Failed to load config: ");
      logger.error(e.what());
    }
  }
};

Config* load_config(string);

// ####################
// CONNECTION MANAGER
// ####################
class ConnectionManager {
public:
  ConnectionManager(Config* cfg) {
    config = cfg;
    logger = Logger();
    logger.info("Created connection manager.\n" + config->to_string());
  }

  ~ConnectionManager() {}

private:
  Logger logger;
  Config* config;
};

#endif
