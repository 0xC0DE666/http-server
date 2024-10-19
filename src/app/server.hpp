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
    logger.info("Loading config.");
    try {
      std::ifstream file(path);
  
      if (!file.is_open()) {
        logger.error("Failed to open config file!");
      }
  
      json data = json::parse(file);
  
      port = data["port"];
      public_dir = data["public_dir"];

      logger.info("Done loading config.");
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
#define BUFFER_SIZE 1024

class ConnectionManager {
public:
  ConnectionManager(Config* cfg) {
    config = cfg;
    logger = Logger();
    logger.info("Created connection manager.\n" + config->to_string());
  }

  ~ConnectionManager() {
    close(server_fd);
  }

  void init() {
    logger.info("Setting up connection manager.");
    // Creating a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        logger.error("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options (optional, but often recommended)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        logger.error("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Define the server address structure
    address.sin_family = AF_INET;  // IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Bind to any IP address
    address.sin_port = htons(config->port);  // Convert port number to network byte order

    // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr*) &address, addrlen) < 0) {
        logger.error("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    logger.info("Done setting up connection manager.");
  }

  void run() {
    // Listen for incoming connections (max backlog of 3)
    if (listen(server_fd, 3) < 0) {
        logger.error("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
        return;
    }
    logger.info("Server listening on port 127.0.0.1:" + std::to_string(config->port) + "...");

    // Accept an incoming connection
    int new_socket;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*) &addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Receive data from the client
    char buffer[BUFFER_SIZE] = {0};
    ssize_t valread = read(new_socket, buffer, BUFFER_SIZE);
    std::cout << "Received: " << buffer << std::endl;

    // Send a response back to the client
    const char *hello = "Hello from server\n";
    send(new_socket, hello, strlen(hello), 0);
    logger.info("Done responding to client.");

    // Close the socket
    close(new_socket);
  }

private:
  Logger logger;
  Config* config;

  int server_fd;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
};

#endif
