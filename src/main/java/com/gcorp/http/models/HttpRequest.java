package com.gcorp.http.models;

import com.gcorp.http.enums.HttpMethod;
import com.gcorp.http.enums.HttpStatusCode;
import com.gcorp.http.enums.HttpVersion;
import com.gcorp.http.exceptions.BadHttpVersionException;
import com.gcorp.http.exceptions.HttpParsingException;

public class HttpRequest extends HttpMessage {
  private HttpMethod method;
  private String target;
  private String version;
  private HttpVersion compatibleVersion;

  public HttpMethod getMethod() {
    return method;
  }

  public void setMethod(String name) throws HttpParsingException {
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

  public void setTarget(String target) throws HttpParsingException {
    if (target == null || target.length() == 0) {
      throw new HttpParsingException(HttpStatusCode.INTERNAL_SERVER_ERROR);
    }
    this.target = target;
  }

  public String getVersion() {
    return version;
  }

  public HttpVersion getCompatibleVersion() {
    return compatibleVersion;
  }

  public void setVersion(String version) throws BadHttpVersionException, HttpParsingException {
    this.version = version;
    this.compatibleVersion = HttpVersion.get(version)
        .orElseThrow(() -> new HttpParsingException(HttpStatusCode.BAD_REQUEST));
  }
}
