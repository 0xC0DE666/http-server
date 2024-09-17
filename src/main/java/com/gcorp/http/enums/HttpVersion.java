package com.gcorp.http.enums;

import java.util.Optional;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.gcorp.http.exceptions.HttpParsingException;

public enum HttpVersion {
  
  HTTP_0_9("HTTP/0.9", 0, 9),
  HTTP_1_0("HTTP/1.0", 1, 0),
  HTTP_1_1("HTTP/1.1", 1, 1),
  // HTTP_2_0("HTTP/2.0", 2, 0),
  // HTTP_3_0("HTTP/3.0", 3, 0),
  ;
  
  public final String LITTERAL;
  public final int MAJOR;
  public final int MINOR;

  HttpVersion(String litteral, int major, int minor) {
    this.LITTERAL = litteral;
    this.MAJOR = major;
    this.MINOR = minor;
  }

  private static final Pattern regex = Pattern.compile("^HTTP/(?<major>\\d+).(?<minor>\\d+)");

  public static Optional<HttpVersion> get(String litteral) throws HttpParsingException {
    Matcher m = regex.matcher(litteral) ;

    if (!m.find() && m.groupCount() != 2) {
      // TODO: dude in video created BadVersionException and threw here.
      throw new HttpParsingException(HttpStatusCode.BAD_REQUEST);
    }

    int major = Integer.parseInt(m.group("major"));
    int minor = Integer.parseInt(m.group("minor"));
    HttpVersion compatible = null;

    for (HttpVersion ver : values()) {
      if (ver.LITTERAL == litteral) {
        // full support
        return Optional.of(ver);
      }
      if (ver.MAJOR == major) {
        compatible = ver;
        if (ver.MINOR == minor) {
          compatible = ver;
        }
      }
    }

    return Optional.ofNullable(compatible);
  }
}
