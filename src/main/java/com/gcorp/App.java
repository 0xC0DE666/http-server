package com.gcorp;

import java.io.IOException;

import com.gcorp.config.ConfigManager;
import com.gcorp.core.ConnectionManager;

/**
 * HTTP server!
 */
public class App {

    public static void main(String[] args) {
      ConfigManager.getInstance().loadConfig("src/main/resources/config.json");

      var conf = ConfigManager.getInstance().getConfig();
      try {
        var connMan = new ConnectionManager(conf);
        connMan.start();
      } catch (IOException e) {
        // TODO: handle properly later
        e.printStackTrace();
      }

      System.out.println("Server listening...");
      System.out.println("port: " + conf.getPort());
      System.out.println("web root: " + conf.getRoot());
    }
}
