#include <map>
#include <vector>
#include <functional>
#include <thread>
#include <netinet/in.h>

#ifndef SERVER_H
#define SERVER_H

using std::string;

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
class Client {
public:
  Client(int id, int cli_fd, std::function<void()> on_cls);

  ~Client();

  void start();
  void print_info();

private:
  Logger logger;
  int id;
  int client_fd;
  std::function<void()> on_close;
  std::thread* thread;

  Client();

  void run();
};

// ####################
// CONNECTION MANAGER
// ####################
#define BUFFER_CAPACITY 1024

class ClientManager {
public:
  ClientManager(const Config* cfg);

  ~ClientManager();

  void start();

private:
  Logger logger;
  const Config* config;
  std::map<int, Client*> open_clients;
  std::vector<int> closed_clients;
  // TODO: Implement below thread to wait and accept client connections.
  // All clients should be joined to this thread.
  // This thread should be joined to the main thread.
  std::thread* client_waiter;
  // TODO: (Maybe) Implement below thread to periodically remove closed clients.
  // This thread should be joined to the main thread.
  std::thread* client_cleaner;

  int server_fd;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  void init();
  void run();

  bool socket_open();
  bool socket_bound();

  void add_client(int client_fd);
  void remove_closed_clients();
};

#endif
