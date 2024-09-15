package com.gcorp;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

import com.gcorp.config.ConfigManager;

/**
 * HTTP server!
 */
public class App {
    public static void main(String[] args) {
        ConfigManager.getInstance().loadConfig("src/main/resources/config.json");

        var conf = ConfigManager.getInstance().getConfig();

        System.out.println("Server listening...");
        System.out.println("port: " + conf.getPort());
        System.out.println("web root: " + conf.getRoot());

        try {
          ServerSocket serverSocket = new ServerSocket(conf.getPort());
          Socket clientSocket = serverSocket.accept();

          InputStream in = clientSocket.getInputStream();
          OutputStream out = clientSocket.getOutputStream();

          String res = httpResponse(landing());
          out.write(res.getBytes());

          in.close();
          out.close();
          clientSocket.close();
          serverSocket.close();

        } catch (IOException e) {
          e.printStackTrace();
        }
    }

    private static String httpResponse(String html) {
      StringBuilder buffer = new StringBuilder();
      final String CRLF = "\n\r"; // 13, 10 ASCII

      buffer.append("HTTP/1.1 200 OK");
      buffer.append(CRLF);
      buffer.append("Content-Length: " + html.getBytes().length + CRLF + CRLF);
      buffer.append(html +  CRLF + CRLF);

      return buffer.toString();
    }

    private static String landing() {
      String html = """
        <html>
          <head>
            <title>Web server from scratch</title>
          </head>
          <body>
            <h1>Http Web Server</h1>
            <p>This page is being served by an HTTP server, written from scratch using TCP sockets in java. :)</p>
          </body>
        </html>
      """;
      return html;
    }
}
