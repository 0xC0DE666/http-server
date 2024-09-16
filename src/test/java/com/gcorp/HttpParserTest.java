package com.gcorp;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;

import com.gcorp.http.HttpMethod;
import com.gcorp.http.HttpParser;
import com.gcorp.http.HttpParsingException;
import com.gcorp.http.HttpRequest;
import com.gcorp.http.HttpStatusCode;

@TestInstance(Lifecycle.PER_CLASS)
public class HttpParserTest {

  private HttpParser parser;

  @BeforeAll
  public void init() {
    parser = new HttpParser();
  }

  @Test
  public void parseRequest_success() {
    try {
      HttpRequest actual = parser.parseRequest(getRequest());
      assertEquals(HttpMethod.GET, actual.getMethod());
    } catch (HttpParsingException e) {
      fail(e);
    }
  }

  @Test
  public void parseRequest_fail_bad_method() {
    try {
      parser.parseRequest(badGetRequest());
      fail();
    } catch (HttpParsingException e) {
      assertEquals(HttpStatusCode.NOT_IMPLEMENTED.CODE, e.getStatus().CODE);
    }
  }


  private InputStream getRequest() {
    String raw =
      "GET / HTTP/1.1\r\n" +
      "Host: localhost:6969\r\n" +
      "Connection: keep-alive\r\n" +
      "Cache-Control: max-age=0\r\n" +
      "sec-ch-ua: \"Chromium\";v=\"129\", \"Not=A?Brand\";v=\"8\"\r\n" +
      "sec-ch-ua-mobile: ?0\r\n" +
      "sec-ch-ua-platform: \"Linux\"\r\n" +
      "Upgrade-Insecure-Requests: 1\r\n" +
      "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/129.0.0.0 Safari/537.36\r\n" +
      "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n" +
      "Sec-Fetch-Site: none\r\n" +
      "Sec-Fetch-Mode: navigate\r\n" +
      "Sec-Fetch-User: ?1\r\n" +
      "Sec-Fetch-Dest: document\r\n" +
      "Accept-Encoding: gzip, deflate, br, zstd\r\n" +
      "Accept-Language: en-GB,en;q=0.9\r\n" +
      "dnt: 1\r\n" +
      "sec-gpc: 1\r\n";

    InputStream in = new ByteArrayInputStream(raw.getBytes(StandardCharsets.US_ASCII));

    return in;
  }

  private InputStream badGetRequest() {
    String raw =
      "GeT / HTTP/1.1\r\n" + // invalid method
      "Host: localhost:6969\r\n" +
      "Accept-Encoding: gzip, deflate, br, zstd\r\n" +
      "Accept-Language: en-GB,en;q=0.9\r\n" +
      "dnt: 1\r\n" +
      "sec-gpc: 1\r\n";

    InputStream in = new ByteArrayInputStream(raw.getBytes(StandardCharsets.US_ASCII));

    return in;
  }

}
