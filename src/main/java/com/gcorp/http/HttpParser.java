package com.gcorp.http;

import java.io.InputStream;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class HttpParser {

  private final static Logger logger = LoggerFactory.getLogger(HttpParser.class);

  public String parseRequest(InputStream in) {
    return "true";
  }
}
