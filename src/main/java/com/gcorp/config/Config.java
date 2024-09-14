package com.gcorp.config;

public class Config {
  private int port;
  private String root;

  public Config() {}

  public Config(int port, String root) {
    this.port = port;
    this.root = root;
  }

  public int getPort() {
      return port;
  }

  public void setPort(int port) {
      this.port = port;
  }

  public String getRoot() {
      return root;
  }

  public void setRoot(String root) {
      this.root = root;
  }
}
