package com.gcorp.config;

public class ConfigurationManager {
  private static ConfigurationManager instance;
  private static Configuration config;
  
  public static ConfigurationManager getInstance() {
    if (instance == null) {
      instance = new ConfigurationManager();
    }
    return instance;
  }

  public void loadConfig(String path) {
  }

  public void getConfig() {
  }
}
