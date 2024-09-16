package com.gcorp.http;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class HttpParser {

  private final static Logger logger = LoggerFactory.getLogger(HttpParser.class);

  private static final int SP = 0x20; // 32 ' '
  private static final int CR = 0x0D; // 13 '\r'
  private static final int LF = 0x0A; // 10 '\n'

  public HttpRequest parseRequest(InputStream in) {
    InputStreamReader reader = new InputStreamReader(in, StandardCharsets.US_ASCII);

    HttpRequest req = new HttpRequest();
    // parseRequestLine(in, req);
    // parseHeaders(in, req);
    // parseBody(in, req);

    return req;
  }

  private void parseRequestLine(InputStreamReader reader, HttpRequest req) throws IOException {
    int _byte;
    while((_byte = reader.read()) >= 0) {
      if (_byte == CR) {
        _byte = reader.read();
        if (_byte == LF) {
          return;
        }
      }
    }  
  }

  private void parseHeaders(InputStreamReader reader, HttpRequest req) {

  }

  private void parseBody(InputStreamReader reader, HttpRequest req) {

  }
}
