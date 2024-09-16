package com.gcorp.http;

public class HttpParsingException extends RuntimeException {
  private final HttpStatusCode code;

  public HttpParsingException(HttpStatusCode code) {
    super(code.MESSAGE);
    this.code = code;
  }

  public HttpStatusCode getCode() {
      return code;
  }
}
