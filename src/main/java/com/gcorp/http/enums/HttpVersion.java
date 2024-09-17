package com.gcorp.http.enums;

import java.util.Optional;
import java.util.regex.Pattern;
import com.gcorp.http.exceptions.BadHttpVersionException;

public enum HttpVersion {

  HTTP_0_9("HTTP/0.9", 0, 9), //
  HTTP_1_0("HTTP/1.0", 1, 0), //
  HTTP_1_1("HTTP/1.1", 1, 1), //
  HTTP_2_0("HTTP/2", 2, 0), //
  HTTP_3_0("HTTP/3", 3, 0);

  public final String LITTERAL;
  public final int MAJOR;
  public final int MINOR;

  HttpVersion(String literal, int major, int minor) {
    this.LITTERAL = literal;
    this.MAJOR = major;
    this.MINOR = minor;
  }

  private static final Pattern singleDigit = Pattern.compile("^HTTP/(?<major>\\d+)");
  private static final Pattern doubleDigit = Pattern.compile("^HTTP/(?<major>\\d+).(?<minor>\\d+)");

  public static Optional<HttpVersion> get(String literal) throws BadHttpVersionException {
    var sinlgeMatch = singleDigit.matcher(literal);
    var doubleMatch = doubleDigit.matcher(literal);

    if (!sinlgeMatch.find() && !doubleMatch.find()) {
      throw new BadHttpVersionException("Malformed HTTP version.");
    }

    int major = doubleMatch.find()//
        ? Integer.parseInt(doubleMatch.group("major"))//
        : Integer.parseInt(sinlgeMatch.group("major"));
    int minor = doubleMatch.find() ? Integer.parseInt(doubleMatch.group("minor")) : 0;
    HttpVersion compatible = null;

    for (HttpVersion ver : values()) {
      if (ver.LITTERAL == literal) {
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
