package com.gcorp.http.exceptions;

public class BadHttpVersionException extends Exception {

  public BadHttpVersionException(String msg) {
    super(msg);
  }

  public BadHttpVersionException(Throwable t) {
    super(t);
  }

  public BadHttpVersionException(String msg, Throwable t) {
    super(msg, t);
  }
}
