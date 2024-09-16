package com.gcorp.http;

public class HttpRequest extends HttpMessage {
  private HttpMethod method;
  private String target;
  private String version;

  HttpRequest() {}

  public HttpMethod getMethod() {
      return method;
  }

  void setMethod(String name) throws HttpParsingException {
    for (HttpMethod method : HttpMethod.values()) {
      if (name.equals(method.name())) {
        this.method = method;
        return;
      }
    }
    throw new HttpParsingException(HttpStatusCode.NOT_IMPLEMENTED);
  }

  public String getTarget() {
    return target;
  }

  void setTarget(String target) throws HttpParsingException {
    if (target == null || target.length() == 0) {
      throw new HttpParsingException(HttpStatusCode.INTERNAL_SERVER_ERROR);
    }
    this.target = target;
  }
}
