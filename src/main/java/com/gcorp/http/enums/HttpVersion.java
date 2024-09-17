package com.gcorp.http.enums;

import java.util.Optional;
import java.util.regex.Pattern;
import com.gcorp.http.exceptions.BadHttpVersionException;

public enum HttpVersion {

  HTTP_0_9("HTTP/0.9", 0, 9), HTTP_1_0("HTTP/1.0", 1, 0), HTTP_1_1("HTTP/1.1", 1, 1),
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

  public static Optional<HttpVersion> get(String litteral) throws BadHttpVersionException {
    var match = regex.matcher(litteral);

    if (!match.find() || match.groupCount() != 2) {
      throw new BadHttpVersionException();
    }

    int major = Integer.parseInt(match.group("major"));
    int minor = Integer.parseInt(match.group("minor"));
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
