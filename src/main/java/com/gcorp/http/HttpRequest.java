package com.gcorp.http;

public class HttpRequest extends HttpMessage {
  private HttpMethod method;
  private String target;
  private String version;

  HttpRequest() {}

  public HttpMethod getMethod() {
      return method;
  }

  // TODO: remove public 
  public void setMethod(HttpMethod method) {
      this.method = method;
  }
}
