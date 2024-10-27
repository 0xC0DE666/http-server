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

int add(int a, int b) {
  return a + b;
}

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
// HTTP
// ####################
// METHOD
const string Http::Method::HEAD = "HEAD";
const string Http::Method::OPTIONS = "OPTIONS";
const string Http::Method::CONNECT = "CONNECT";
const string Http::Method::TRACE = "TRACE";

const string Http::Method::GET = "GET";
const string Http::Method::POST = "POST";
const string Http::Method::PUT = "PUT";
const string Http::Method::PATCH = "PATCH";
const string Http::Method::DELETE = "DELETE";

const int Http::Method::MAX_LENGTH = 7;

// VERSION
const Http::Version Http::Version::v0_9 = Http::Version("HTTP/0.9", 0, 9);
const Http::Version Http::Version::v1_0 = Http::Version("HTTP/1.0", 1, 0);
const Http::Version Http::Version::v1_1 = Http::Version("HTTP/1.1", 1, 1);
const Http::Version Http::Version::v2_0 = Http::Version("HTTP/2", 2, 0);
const Http::Version Http::Version::v3_0 = Http::Version("HTTP/3", 3, 0);

// STATUS
// 400 client
const Http::Status Http::Status::BAD_REQUEST = Http::Status(400, "Bad Request");
const Http::Status Http::Status::NOT_FOUND = Http::Status(404, "Not Found");
const Http::Status Http::Status::METHOD_NOT_ALLOWED = Http::Status(405, "Method Not Allowed");
const Http::Status Http::Status::URI_TOO_LONG = Http::Status(414, "URI Too Long");
// 500 server
const Http::Status Http::Status::INTERNAL_SERVER_ERROR = Http::Status(500, "Internal Server Error");
const Http::Status Http::Status::NOT_IMPLEMENTED = Http::Status(501, "Not Implemented");
const Http::Status Http::Status::VERSION_NOT_SUPPORTED = Http::Status(505, "Version Not Supported");

// ####################
// CLIENT
// ####################
Client::Client(int id, int cli_fd) {
  id = id;
  client_fd = cli_fd;
  thread = nullptr;
  logger.info("Client connected.");
}

Client::~Client() {
  // TODO: check this
  logger.info("Free client");
  try {
    close(client_fd);
    if (thread) {
      delete thread;
    }
  } catch(std::runtime_error& e) {
    logger.info("AAAAAAAAAAAAAAAAAAAAa");
    logger.info(e.what());
  }
}

void Client::run() {
  thread = new std::thread(&Client::exec, this);
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

void Client::exec() {
  while (true) {
    // Receive data from the client
    char buffer[BUFFER_SIZE] = {0};
    ssize_t byte_count = read(client_fd, buffer, BUFFER_SIZE);
    if (byte_count < 0) {
      logger.error("Failed to read bytes: " + std::to_string(errno));
      logger.error(strerror(errno));
      return;
    }
    logger.info("Received: " + string(buffer));

    // Send a response back to the client
    const char *hello = "Hello from server\n";
    send(client_fd, hello, strlen(hello), 0);
    logger.info("Done responding to client.");

    if (0 == strcmp(buffer, "exit")) {
      logger.info("EXIT");
      // exit();
      break;
    }
  }
  // TODO: check this
  delete this;
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
  int id = clients.size();
  Client* cli = new Client(id, client_fd);
  clients[id] = cli;
  cli->print_info();
  cli->run();
}
