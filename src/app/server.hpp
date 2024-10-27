#include <map>
#include <thread>
#include <netinet/in.h>

#ifndef SERVER_H
#define SERVER_H

using std::string;

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
  const int PORT;
  const string WWW_DIR;

  Config(const int port, const string pub_dir) : PORT(port), WWW_DIR(pub_dir) {};

  string to_string();
  static Config* load();
  static Config* load(string path);

private:
  Config();
};

Config* load_config(string);

// ####################
// CLIENT
// ####################
#define BUFFER_SIZE 1024

class Client {
public:
  Client(int id, int cli_fd);

  ~Client();

  void run();
  void print_info();

private:
  Logger logger;
  int id;
  int client_fd;
  std::thread* thread;

  Client();

  void exec();
};

// ####################
// CONNECTION MANAGER
// ####################
#define BUFFER_SIZE 1024

class ClientManager {
public:
  ClientManager(const Config* cfg);

  ~ClientManager();

  void run();

private:
  Logger logger;
  const Config* config;
  std::map<int, Client*> clients;

  int server_fd;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  void init();
  bool socket_open();
  bool socket_bound();

  void add_client(int client_fd);
  void remove_client(int id);
};

#endif
