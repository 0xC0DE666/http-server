package com.gcorp;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;
import java.util.NoSuchElementException;
import org.junit.jupiter.api.Test;
import com.gcorp.http.enums.HttpVersion;
import com.gcorp.http.exceptions.BadHttpVersionException;

public class HttpVersionTest {

  @Test
  public void getVersion_failBadVersion() {
    try {
      HttpVersion.get("HTTP/1").orElseThrow();
      fail();
    } catch (BadHttpVersionException e) {
      assertTrue(true);
    }
  }

  @Test
  public void getVersion_failNoVersionFound() {
    try {
      var actual = HttpVersion.get("TTP/9.9").orElseThrow();
      fail();
    } catch (BadHttpVersionException e) {
    } catch (NoSuchElementException e) {
      assertTrue(true);
    }
  }

  @Test
  public void getVersion_successExact() {
    try {
      var actual = HttpVersion.get("HTTP/1.0").orElseThrow();
      assertNotNull(actual);
      assertEquals(1, actual.MAJOR);
      assertEquals(0, actual.MINOR);
    } catch (BadHttpVersionException e) {
      fail("failed to get EXACT match HttpVersion.");
    }
  }

  @Test
  public void getVersion_successPartial() {
    try {
      var actual = HttpVersion.get("HTTP/0.1").orElseThrow();
      assertEquals(0, actual.MAJOR);
      assertEquals(9, actual.MINOR);
    } catch (BadHttpVersionException e) {
      fail("failed to get PARTIAL match HttpVersion.");
    }
  }
}
