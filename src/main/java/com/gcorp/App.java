package com.gcorp;

import java.io.IOException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.gcorp.config.ConfigManager;
import com.gcorp.core.ConnectionManager;
import com.gcorp.http.HttpMethod;

/**
 * HTTP server!
 */
public class App {

  private final static Logger logger = LoggerFactory.getLogger(App.class);

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

    logger.info("Server listening...");
    logger.info("port: " + conf.getPort());
    logger.info("web root: " + conf.getRoot());
  }
}
