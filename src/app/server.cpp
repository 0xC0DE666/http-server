#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <iostream>
#include <fstream>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../lib/json.hpp"
#include "./server.hpp"

using nlohmann::json;

// ####################
// LOGGER
// ####################
Logger::Logger() {
  level = LogLevel::INFO;
}

Logger::Logger(LogLevel lvl) {
  level = lvl;
}

Logger::~Logger() {}

void Logger::error(const string& msg) {
  if (level < LogLevel::ERROR) return;
  std::cerr << msg << std::endl;
}

void Logger::warning(const string& msg) {
  if (level < LogLevel::WARNING) return;
  std::cout << msg << std::endl;
}

void Logger::info(const string& msg) {
  if (level < LogLevel::INFO) return;
  std::cout << msg << std::endl;
}

void Logger::debug(const string& msg) {
  if (level < LogLevel::DEBUG) return;
  std::cout << msg << std::endl;
}

void Logger::verbose(const string& msg) {
  if (level < LogLevel::VERBOSE) return;
  std::cout << msg << std::endl;
}

// ####################
// CONFIG
// ####################
string Config::to_string() {
  return std::to_string(PORT) + " " + WWW_DIR;
}

Config* Config::load() {
  try {
    const char* str_port = std::getenv("PORT");
    const char* www_dir = std::getenv("WWW_DIR");
    if (str_port == nullptr || www_dir == nullptr) {
      string err = string("Read env vars PORT and WWW_DIR failed: " + std::to_string(errno));
      perror(err.c_str());
      perror(strerror(errno));

      return nullptr;
    }

    int port = std::atoi(str_port);
    if (port == 0) {
      string err = string("Convert PORT to int failed: " + std::to_string(errno));
      perror(err.c_str());
      perror(strerror(errno));

      return nullptr;
    }
    return new Config(port, www_dir);
  } catch (const std::runtime_error& e) {
    perror("Failed to load config: ");
    perror(e.what());
  }
  return nullptr;
}

Config* Config::load(string path) {
  try {
    std::ifstream file(path);

    if (!file.is_open()) {

      string err = string("Open config file failed: " + std::to_string(errno) + "\n" + path);
      perror(err.c_str());
    }

    json data = json::parse(file);

    return new Config(data["port"], data["www_dir"]);
  } catch (const std::runtime_error& e) {
    perror("Failed to load config: ");
    perror(e.what());
  }
  return nullptr;
}

// ####################
// CLIENT
// ####################
Client::Client(int id, int cli_fd, std::function<void()> on_cls) {
  id = id;
  client_fd = cli_fd;
  on_close = on_cls;
  thread = nullptr;
  logger.info("Client connected: " + std::to_string(id));
}

Client::~Client() {
  logger.info("Free client");
  close(client_fd);

  if (thread && thread->joinable()) {
    thread->join();
    delete thread;
  }
}

void Client::start() {
  if (thread && thread->joinable()) {
    thread->join();
    delete thread;
  }
  thread = new std::thread(&Client::run, this);
}

void Client::print_info() {
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);

  // Get the client socket information
  int e = getpeername(client_fd, (struct sockaddr*)&addr, &addr_len);
  if (e < 0) {
    logger.error("Print socket info failed: " + std::to_string(errno));
    logger.error(strerror(errno));
    return;
  }
  char ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
  logger.info("Client IP Address: " + string(ip));
  logger.info("Client Port: " + std::to_string(ntohs(addr.sin_port)));
}

void Client::run() {
  while (true) {
    char buffer[BUFFER_CAPACITY] = {0};
    // NOTE: If there is more than 1023 bytes, this reads multiple times.
    ssize_t byte_count = recv(client_fd, buffer, BUFFER_CAPACITY - 1, 0);

    // Error reading bytes.
    if (byte_count < 0) {
      logger.error("Failed to read bytes: " + std::to_string(errno));
      logger.error(strerror(errno));
      return;
    }

    // Connection closed.
    if (byte_count == 0) {
      on_close();
      return;
    }

    // Success reading bytes.
    if (byte_count > 0) {
      buffer[byte_count] = '\0';
    }
    logger.info("Received: " + string(buffer));

    // Close connection.
    if (0 == strcmp(buffer, "close")) {
      on_close();
      logger.info("closed client");
      break;
    }

    // Send a response back to the client
    const char *hello = "Hello from server\n";
    send(client_fd, hello, strlen(hello), 0);
    logger.info("Done responding to client.");
  }
}

// ####################
// CLIENT MANAGER
// ####################
ClientManager::ClientManager(const Config* cfg) {
  logger = Logger();
  config = cfg;
  logger.info("Created client manager.");
  init();
}

ClientManager::~ClientManager() {
  close(server_fd);
}

void ClientManager::start() {
  client_waiter = new std::thread(&ClientManager::run, this);
  if (client_waiter && client_waiter->joinable()) {
    client_waiter->join();
  }
}

void ClientManager::init() {
  logger.info("Setting up client manager.");
  int e;
  // Creating a socket file descriptor
  e = server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (e <= 0) {
    logger.error("Create socket failed: " + std::to_string(errno));
    logger.error(strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Set socket options (optional, but often recommended)
  int opt = 1;
  e = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
  if (e) {
    logger.error("Set socket options failed: " + std::to_string(errno));
    logger.error(strerror(errno));
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Define the server address structure
  address.sin_family = AF_INET;  // IPv4
  address.sin_addr.s_addr = INADDR_ANY;  // Bind to any IP address
  address.sin_port = htons(config->PORT);  // Convert port number to network byte order

  // Bind the socket to the specified IP and port
  e = bind(server_fd, (struct sockaddr*) &address, addrlen);
  if (e < 0) {
    logger.error("Bind socket failed: " + std::to_string(errno));
    logger.error(strerror(errno));
    close(server_fd);
    exit(EXIT_FAILURE);
  }
  logger.info("Done setting up client manager.");
}

void ClientManager::run() {
  // Listen for incoming connections (max backlog of 3)
  int e = listen(server_fd, 3);
  if (e < 0) {
    logger.error("Listen failed: " + std::to_string(errno));
    logger.error(strerror(errno));
    close(server_fd);
    exit(EXIT_FAILURE);
  }
  logger.info("Server listening on port 127.0.0.1:" + std::to_string(config->PORT) + "...");

  while (socket_bound() && socket_open()) {
    // Accept an incoming connection
    remove_closed_clients();
    int client_fd = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &addrlen);
    e = client_fd;
    if (e < 0) {
      logger.error("Accept socket failed: " + std::to_string(errno));
      logger.error(strerror(errno));
      close(server_fd);
      exit(EXIT_FAILURE);
    }
    add_client(client_fd);
  }
}

bool ClientManager::socket_open() {
  return server_fd >= 0;
}

bool ClientManager::socket_bound() {
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);

  int e = getsockname(server_fd, (struct sockaddr*)&addr, &addrlen);
  if (e < 0) {
    if (errno == ENOTCONN || errno == EINVAL || errno == EBADF) {
      return false;
    }
  }
  return true;
}

void ClientManager::add_client(int client_fd) {
  int id = open_clients.size() + 1;
  Client* cli = new Client(id, client_fd, [this, id]() {
    closed_clients.push_back(id);
  });
  open_clients[id] = cli;
  cli->print_info();
  cli->start();
}

void ClientManager::remove_closed_clients() {
  for (int i = 0; i < closed_clients.size(); ++i) {
    int id = closed_clients[i];
    delete open_clients[id];
    open_clients.erase(id);
  }
}
