package com.gcorp.core;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.gcorp.config.Config;
import com.gcorp.util.Http;

public class ConnectionManager extends Thread {

  private final static Logger logger = LoggerFactory.getLogger(ConnectionManager.class);

  private Config config;
  private ServerSocket socket;

  public ConnectionManager(Config config) throws IOException {
    super();
    this.config = config; 
    socket = new ServerSocket(config.getPort());
  }
  
  @Override
  public void run() {
    try {
      Socket client = socket.accept();
      logger.info("new client: " + client.getInetAddress());

      InputStream in = client.getInputStream();
      OutputStream out = client.getOutputStream();

      String res = Http.response(Http.landing());
      out.write(res.getBytes());

      in.close();
      out.close();
      client.close();
      socket.close();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
