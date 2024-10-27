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
// HTTP
// ####################
namespace Http {
  // METHOD
  class Method {
  public:
    static const string HEAD;
    static const string OPTIONS;
    static const string CONNECT;
    static const string TRACE;

    static const string GET;
    static const string POST;
    static const string PUT;
    static const string PATCH;
    static const string DELETE;

    static const int MAX_LENGTH;

  private:
    Method();
  };

  // VERSION
  class Version {
  public:
    const string LITTERAL;
    const int MAJOR;
    const int MINOR;

    static const Version v0_9;
    static const Version v1_0;
    static const Version v1_1;
    static const Version v2_0;
    static const Version v3_0;

  private:
    Version();
    Version(const string litteral, const int major, const int minor) : LITTERAL(litteral), MAJOR(major), MINOR(minor){};
  };

  // STATUS
  class Status {
  public:
    const int CODE;
    const string MESSAGE;

    // 400 client
    static const Status BAD_REQUEST;
    static const Status NOT_FOUND;
    static const Status METHOD_NOT_ALLOWED;
    static const Status URI_TOO_LONG;

    // 500 server
    static const Status INTERNAL_SERVER_ERROR;
    static const Status NOT_IMPLEMENTED;
    static const Status VERSION_NOT_SUPPORTED;

  private:
    Status();
    Status(const int code, const string msg) : CODE(code), MESSAGE(msg) {};
  };
}

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
