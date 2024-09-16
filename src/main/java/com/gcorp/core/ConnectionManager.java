package com.gcorp.core;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.gcorp.config.Config;

public class ConnectionManager extends Thread {

  private final static Logger logger = LoggerFactory.getLogger(ConnectionManager.class);

  private Config config;
  private ServerSocket socket;

  public ConnectionManager(Config config) throws IOException {
    this.config = config; 
    socket = new ServerSocket(config.getPort());
  }
  
  @Override
  public void run() {
    try {
      while (socket.isBound() && !socket.isClosed()) {
        Socket conn = socket.accept();
        logger.info("new client: " + conn.getInetAddress());
        // new ClientHandler(client).run(); this runs on the same thread for some reason...
        var client = new ClientHandler(conn);
        client.start();
      }
    } catch (IOException e) {
      logger.error("error setting up client handler: ", e);
    } finally {
      close();
    }
  }

  private void close() {
    try {
      if (socket != null) socket.close();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
