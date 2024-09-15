package com.gcorp.util;

public class Http {

  public static String response(String html) {
    StringBuilder buffer = new StringBuilder();
    final String CRLF = "\n\r"; // 13, 10 ASCII

    buffer.append("HTTP/1.1 200 OK");
    buffer.append(CRLF);
    buffer.append("Content-Length: " + html.getBytes().length + CRLF + CRLF);
    buffer.append(html +  CRLF + CRLF);

    return buffer.toString();
  }

  public static String landing() {
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
