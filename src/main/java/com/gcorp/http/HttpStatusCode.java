package com.gcorp.http;

public enum HttpStatusCode {
  
  // 400 client
  BAD_REQUEST(400, "Bad Request"),
  METHOD_NOT_ALLOWED(405, "Method Not Allowed"),
  URI_TOO_LONG(414, "URI Too Long"),
  // 500 server
  INTERNAL_SERVER_ERROR(500, "Internal Server Error"),
  NOT_IMPLEMENTED(501, "Not Implemented"),
  ;

  public final int CODE;
  public final String MESSAGE;

  HttpStatusCode(int code, String message) {
    this.CODE = code;
    this.MESSAGE = message;
  }
}
