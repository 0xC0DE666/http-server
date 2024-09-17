package com.gcorp.http.enums;

public enum HttpMethod {
  HEAD, GET, POST, PUT, DELETE;

  public static final int MAX_LENGTH;

  static {
    int max = 0;
    for (HttpMethod method : values()) {
      if (method.name().length() > max) {
        max = method.name().length();
      }
    }
    MAX_LENGTH = max;
  }
}
