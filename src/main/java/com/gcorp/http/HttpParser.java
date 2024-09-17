package com.gcorp.http;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.regex.Pattern;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import com.gcorp.http.enums.HttpMethod;
import com.gcorp.http.enums.HttpStatusCode;
import com.gcorp.http.exceptions.BadHttpVersionException;
import com.gcorp.http.exceptions.HttpParsingException;
import com.gcorp.http.models.HttpRequest;

public class HttpParser {

  private final static Logger logger = LoggerFactory.getLogger(HttpParser.class);

  private static final int SP = 0x20; // 32 ' '
  private static final int CR = 0x0D; // 13 '\r'
  private static final int LF = 0x0A; // 10 '\n'

  public HttpRequest parseRequest(InputStream in) throws HttpParsingException {
    InputStreamReader reader = new InputStreamReader(in, StandardCharsets.US_ASCII);

    HttpRequest req = new HttpRequest();
    try {
      parseRequestLine(reader, req);
      parseHeaders(reader, req);
      // parseBody(reader, req);
    } catch (IOException e) {
      // TODO: handle this
      e.printStackTrace();
    }

    return req;
  }

  private void parseRequestLine(InputStreamReader reader, HttpRequest req)
      throws IOException, HttpParsingException {
    StringBuilder buffer = new StringBuilder();
    int _byte;
    boolean method = false;
    boolean target = false;

    while ((_byte = reader.read()) >= 0) {

      if (_byte == CR) {
        _byte = reader.read();

        if (_byte != LF) {
          throw new HttpParsingException(HttpStatusCode.BAD_REQUEST);
        }

        if (_byte == LF) {
          logger.debug("request line VERSION: {}", buffer.toString());
          try {
            req.setVersion(buffer.toString());
          } catch (BadHttpVersionException e) {
            throw new HttpParsingException(HttpStatusCode.BAD_REQUEST);
          }

          if (!method || !target) {
            throw new HttpParsingException(HttpStatusCode.BAD_REQUEST);
          }
          return;
        }
      }

      if (_byte == SP) {
        if (!method) {
          logger.debug("request line METHOD: {}", buffer.toString());
          req.setMethod(buffer.toString());
          method = true;
        } else if (!target) {
          logger.debug("request line TARGET: {}", buffer.toString());
          req.setTarget(buffer.toString());
          target = true;
        } else {
          throw new HttpParsingException(HttpStatusCode.BAD_REQUEST);
        }
        buffer.delete(0, buffer.length());
      } else {
        buffer.append((char) _byte);
        if (!method && buffer.length() == HttpMethod.MAX_LENGTH) {
          throw new HttpParsingException(HttpStatusCode.NOT_IMPLEMENTED);
        }
      }
    }
  }

  private void parseHeaders(InputStreamReader reader, HttpRequest req)
      throws IOException, HttpParsingException {
    var buffer = new StringBuilder();
    int _byte;
    var crlf = false;

    while ((_byte = reader.read()) >= 0) {

      if (_byte == CR) {
        _byte = reader.read();

        if (_byte == LF) {
          if (!crlf) {
            crlf = true;
            parseHeader(buffer, req);
            buffer.delete(0, buffer.length());
          } else {
            return;
          }
        } else {
          throw new HttpParsingException(HttpStatusCode.BAD_REQUEST);
        }
      } else {
        crlf = false;
        buffer.append((char) _byte);
      }
    }
  }

  private void parseHeader(StringBuilder builder, HttpRequest req) throws HttpParsingException {
    var header = builder.toString();
    // var pattern = Pattern.compile(
    // "^(?<name>[!#$%&’*+\\-./^_‘|˜\\dA-Za-z]+):\\s?(?<value>[!#$%&’*+\\-./^_‘|˜(),:;<=>?@[\\\\]{}
    // \\dA-Za-z]+)\\s?$"//
    // );
    var pattern = Pattern.compile("(?m)^([^:]+):\\s*(.+)$");

    var match = pattern.matcher(header);

    if (!match.matches()) {
      throw new HttpParsingException(HttpStatusCode.BAD_REQUEST);
    }

    var key = match.group(1);
    var value = match.group(2);
    req.putHeader(key, value);
  }

  private void parseBody(InputStreamReader reader, HttpRequest req) {

  }
}
