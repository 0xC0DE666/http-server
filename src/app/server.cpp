#include <cerrno>
#include <cstdio>
#include <cstring>
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
  return std::to_string(PORT) + " " + PUBLIC_DIR;
}

Config* Config::load(string path) {
  try {
    std::ifstream file(path);

    if (!file.is_open()) {
      perror("Failed to open config file!");
    }

    json data = json::parse(file);

    return new Config(data["port"], data["public_dir"]);
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
// CONNECTION MANAGER
// ####################
ClientManager::ClientManager(const Config* cfg) {
  config = cfg;
  logger = Logger();
  logger.info("Created connection manager.\n");
}

ClientManager::~ClientManager() {
  close(server_fd);
}

void ClientManager::init() {
  logger.info("Setting up connection manager.");
  int e;
  // Creating a socket file descriptor
  e = server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (e == 0) {
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
  logger.info("Done setting up connection manager.");
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

  while (true) {
    // Accept an incoming connection
    int socket = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &addrlen);
    e = socket;
    if (e < 0) {
      logger.error("Accept socket failed: " + std::to_string(errno));
      logger.error(strerror(errno));
      close(server_fd);
      exit(EXIT_FAILURE);
    }
    Client* c = new Client(socket);
    c->run();
    logger.info("----------------");
  }
}

// ####################
// CLIENT
// ####################
Client::Client(int sckt) {
  socket = sckt; 
  thread = nullptr;
  logger.info("Client connected.");
}

Client::~Client() {
  close(socket);
  if (thread) {
    delete thread;
  }
}

void Client::run() {
  thread = new std::thread(&Client::exec, this);
}

void Client::exec() {
  while (true) {
    // Receive data from the client
    char buffer[BUFFER_SIZE] = {0};
    ssize_t valread = read(socket, buffer, BUFFER_SIZE);
    logger.info("Received: " + string(buffer));

    // Send a response back to the client
    const char *hello = "Hello from server\n";
    send(socket, hello, strlen(hello), 0);
    logger.info("Done responding to client.");

    if (0 == strcmp(buffer, "exit")) {
      logger.info("EXIT");
      break;
    }
  }
  close(socket);
}
