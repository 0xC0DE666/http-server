package com.gcorp;

import com.gcorp.config.ConfigManager;

/**
 * HTTP server!
 */
public class App {
    public static void main(String[] args) {
        System.out.println("Server starting...");
        ConfigManager.getInstance().loadConfig("src/main/resources/config.json");
        var conf = ConfigManager.getInstance().getConfig();
        System.out.println(conf.getPort() + " " + conf.getRoot());
    }
}
