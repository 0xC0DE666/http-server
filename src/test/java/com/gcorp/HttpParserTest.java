package com.gcorp;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.fail;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import com.gcorp.http.HttpParser;
import com.gcorp.http.enums.HttpMethod;
import com.gcorp.http.enums.HttpStatusCode;
import com.gcorp.http.enums.HttpVersion;
import com.gcorp.http.exceptions.HttpParsingException;
import com.gcorp.http.models.HttpRequest;

@TestInstance(Lifecycle.PER_CLASS)
public class HttpParserTest {

  private HttpParser parser;

  @BeforeAll
  public void init() {
    parser = new HttpParser();
  }

  @Test
  public void parseRequest_failInvalidMethod() {
    try {
      parser.parseRequest(badGetRequest_invalidMethod());
      fail();
    } catch (HttpParsingException e) {
      assertEquals(HttpStatusCode.NOT_IMPLEMENTED.CODE, e.getStatus().CODE);
    }
  }

  @Test
  public void parseRequest_failMethodTooLong() {
    try {
      parser.parseRequest(badGetRequest_methodTooLong());
      fail();
    } catch (HttpParsingException e) {
      assertEquals(HttpStatusCode.NOT_IMPLEMENTED.CODE, e.getStatus().CODE);
    }
  }

  @Test
  public void parseRequest_failMoreThan3PartsInRequestLine() {
    try {
      parser.parseRequest(badGetRequest_moreThan3PartsInRequestLine());
      fail();
    } catch (HttpParsingException e) {
      assertEquals(HttpStatusCode.BAD_REQUEST.CODE, e.getStatus().CODE);
    }
  }

  @Test
  public void parseRequest_failEmptyRequestLine() {
    try {
      parser.parseRequest(badGetRequest_emptyRequestLine());
      fail();
    } catch (HttpParsingException e) {
      assertEquals(HttpStatusCode.BAD_REQUEST.CODE, e.getStatus().CODE);
    }
  }

  @Test
  public void parseRequest_failNoLF() {
    try {
      parser.parseRequest(badGetRequest_noLF());
      fail();
    } catch (HttpParsingException e) {
      assertEquals(HttpStatusCode.BAD_REQUEST.CODE, e.getStatus().CODE);
    }
  }

  @Test
  public void parseRequest_failMalformedVersion() {
    try {
      parser.parseRequest(badGetRequest_malformedVersion());
      fail();
    } catch (HttpParsingException e) {
      assertEquals(HttpStatusCode.BAD_REQUEST.CODE, e.getStatus().CODE);
    }
  }

  @Test
  public void parseRequest_failBadVersion() {
    try {
      parser.parseRequest(badGetRequest_badVersion());
      fail();
    } catch (HttpParsingException e) {
      assertEquals(HttpStatusCode.BAD_REQUEST.CODE, e.getStatus().CODE);
    }
  }

  @Test
  public void parseRequest_success() {
    try {
      HttpRequest actual = parser.parseRequest(getRequest());

      assertNotNull(actual);
      assertNotNull(actual.getMethod());
      assertNotNull(actual.getTarget());
      assertNotNull(actual.getVersion());
      assertNotNull(actual.getHeaders());

      // status line
      assertEquals(HttpMethod.GET, actual.getMethod());
      assertEquals("/", actual.getTarget());
      assertEquals(HttpVersion.HTTP_1_1.LITTERAL, actual.getVersion().LITTERAL);
      assertEquals(HttpVersion.HTTP_1_1.LITTERAL, actual.getLiteralVersion());

      // headers
      assertEquals(17, actual.getHeaders().size());
      assertEquals("localhost:6969", actual.getHeader("Host").get());
      assertEquals("keep-alive", actual.getHeader("Connection").get());
      assertEquals("1", actual.getHeader("dnt").get());
    } catch (HttpParsingException e) {
      fail(e);
    }
  }

  private InputStream getRequest() {
    String raw = "GET / HTTP/1.1\r\n" + "Host: localhost:6969\r\n" + "Connection: keep-alive\r\n"
        + "Cache-Control: max-age=0\r\n"
        + "sec-ch-ua: \"Chromium\";v=\"129\", \"Not=A?Brand\";v=\"8\"\r\n"
        + "sec-ch-ua-mobile: ?0\r\n" + "sec-ch-ua-platform: \"Linux\"\r\n"
        + "Upgrade-Insecure-Requests: 1\r\n"
        + "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/129.0.0.0 Safari/537.36\r\n"
        + "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
        + "Sec-Fetch-Site: none\r\n" + "Sec-Fetch-Mode: navigate\r\n" + "Sec-Fetch-User: ?1\r\n"
        + "Sec-Fetch-Dest: document\r\n" + "Accept-Encoding: gzip, deflate, br, zstd\r\n"
        + "Accept-Language: en-GB,en;q=0.9\r\n" + "dnt: 1\r\n" + "sec-gpc: 1\r\n";

    InputStream in = new ByteArrayInputStream(raw.getBytes(StandardCharsets.US_ASCII));

    return in;
  }

  private InputStream badGetRequest_invalidMethod() {
    String raw = "G_T / HTTP/1.1\r\n"; // invalid method
    return new ByteArrayInputStream(raw.getBytes(StandardCharsets.US_ASCII));
  }

  private InputStream badGetRequest_methodTooLong() {
    String raw = "G________T / HTTP/1.1\r\n"; // method too long
    return new ByteArrayInputStream(raw.getBytes(StandardCharsets.US_ASCII));
  }

  private InputStream badGetRequest_moreThan3PartsInRequestLine() {
    String raw = "GET / HTTP/1.1 ABC\r\n"; // more than 3 parts in request line
    return new ByteArrayInputStream(raw.getBytes(StandardCharsets.US_ASCII));
  }

  private InputStream badGetRequest_emptyRequestLine() {
    String raw = "\r\n"; // empty request line
    return new ByteArrayInputStream(raw.getBytes(StandardCharsets.US_ASCII));
  }

  private InputStream badGetRequest_noLF() {
    String raw = "GET / HTTP/1.1\r"; // CR no LF
    return new ByteArrayInputStream(raw.getBytes(StandardCharsets.US_ASCII));
  }

  private InputStream badGetRequest_malformedVersion() {
    String raw = "GET / HTTP/\r\n"; // Malformed version
    return new ByteArrayInputStream(raw.getBytes(StandardCharsets.US_ASCII));
  }

  private InputStream badGetRequest_badVersion() {
    String raw = "GET / HTTP/9\r\n"; // bad http version
    return new ByteArrayInputStream(raw.getBytes(StandardCharsets.US_ASCII));
  }
}
