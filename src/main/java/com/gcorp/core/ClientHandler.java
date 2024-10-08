package com.gcorp.core;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.gcorp.http.HttpParser;
import com.gcorp.util.Http;

public class ClientHandler extends Thread {

  private final static Logger logger = LoggerFactory.getLogger(ClientHandler.class);
 
  private Socket socket;
  private InputStream in;
  private OutputStream out;

  private HttpParser parser;

  public ClientHandler(Socket socket) {
    try {
      this.socket = socket;
      this.in = socket.getInputStream();
      this.out = socket.getOutputStream();
      this.parser = new HttpParser();
    } catch (IOException e) {
      e.printStackTrace(); 
    }
  }
  
  @Override
  public void run() {
    try {
      // parser.parseRequest(in);

      String res = Http.response(Http.landing());
      out.write(res.getBytes());
      logger.info("reguest handled: " + socket.getInetAddress());
    } catch (Exception e) {
      logger.error("error handling client request: ", e);
    } finally {
      close();
    }
  }

  private void close() {
    try {
      if (in != null) in.close();
      if (out != null) out.close();
      if (socket != null) socket.close();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
