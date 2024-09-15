package com.gcorp.config;

import java.io.FileReader;
import java.io.IOException;

import com.gcorp.util.Json;

public class ConfigManager {
  private static ConfigManager instance;
  private static Config config;
  
  public static ConfigManager getInstance() {
    if (instance == null) {
      instance = new ConfigManager();
    }
    return instance;
  }

  public void loadConfig(String path) {
    FileReader reader;
    StringBuffer buffer;
    try {
      reader = new FileReader(path);
      buffer = new StringBuffer();

      int i;
      while ((i = reader.read()) != -1) {
        buffer.append((char) i);
      }
      reader.close();
    } catch (IOException e) {
      throw new ConfigException(e);
    }

    try {
      var conf = Json.parse(buffer.toString());
      config = Json.toPOJO(conf, Config.class);
    } catch (IOException e) {
      throw new ConfigException("Error parsing config file: " + path, e);
    }
  }

  public Config getConfig() {
    if (config == null) {
      throw new ConfigException("Config has not been loaded.");
    }
    return config;
  }
}
