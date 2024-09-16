package com.gcorp.http;

public class HttpParsingException extends Exception {
  private final HttpStatusCode status;

  public HttpParsingException(HttpStatusCode status) {
    super(status.MESSAGE);
    this.status = status;
  }

  public HttpStatusCode getStatus() {
      return status;
  }
}
