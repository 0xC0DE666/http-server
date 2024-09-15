package com.gcorp.core;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

import com.gcorp.config.Config;
import com.gcorp.util.Http;

public class ConnectionManager extends Thread {

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
      System.out.println("new client: " + client.getInetAddress().toString());

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
