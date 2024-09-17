package com.gcorp.http.models;

import java.util.HashMap;
import java.util.Optional;
import java.util.Set;
import com.gcorp.http.enums.HttpMethod;
import com.gcorp.http.enums.HttpStatusCode;
import com.gcorp.http.enums.HttpVersion;
import com.gcorp.http.exceptions.BadHttpVersionException;
import com.gcorp.http.exceptions.HttpParsingException;

public class HttpRequest extends HttpMessage {
  private HttpMethod method;
  private String target;
  private HttpVersion version;
  private String literalVersion;
  private HashMap<String, String> headers = new HashMap<>();

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

  public HttpVersion getVersion() {
    return version;
  }

  public void setVersion(String version) throws BadHttpVersionException, HttpParsingException {
    this.literalVersion = version;
    this.version = HttpVersion.get(version)
        .orElseThrow(() -> new HttpParsingException(HttpStatusCode.BAD_REQUEST));
  }

  public String getLiteralVersion() {
    return literalVersion;
  }

  public Optional<String> getHeader(String key) {
    return Optional.ofNullable(this.headers.get(key));
  }

  public void putHeader(String key, String value) {
    this.headers.put(key, value);
  }

  public Set<String> getHeaders() {
    return this.headers.keySet();
  }
}
