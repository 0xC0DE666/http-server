package com.gcorp.http.enums;

public enum HttpStatusCode {

  // 400 client
  BAD_REQUEST(400, "Bad Request"), //
  METHOD_NOT_ALLOWED(405, "Method Not Allowed"), //
  URI_TOO_LONG(414, "URI Too Long"), //
  // 500 server
  INTERNAL_SERVER_ERROR(500, "Internal Server Error"), //
  NOT_IMPLEMENTED(501, "Not Implemented"), //
  VERSION_NOT_SUPPORTED(505, "Version Not Supported"),//
  ;

  public final int CODE;
  public final String MESSAGE;

  HttpStatusCode(int code, String message) {
    this.CODE = code;
    this.MESSAGE = message;
  }
}
