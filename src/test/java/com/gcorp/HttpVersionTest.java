package com.gcorp;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.NoSuchElementException;
import org.junit.jupiter.api.Test;
import com.gcorp.http.enums.HttpVersion;
import com.gcorp.http.exceptions.BadHttpVersionException;

public class HttpVersionTest {

  @Test
  public void getVersion_failBadVersion() {
    try {
      HttpVersion.get("HTTP/").orElseThrow();
      fail();
    } catch (BadHttpVersionException e) {
      assertTrue(true);
    }
  }

  @Test
  public void getVersion_failNoVersionFound() {
    try {
      HttpVersion.get("TTP/9.9").orElseThrow();
      fail();
    } catch (BadHttpVersionException e) {
    } catch (NoSuchElementException e) {
      assertTrue(true);
    }
  }

  @Test
  public void getVersion_successDoubleExact() {
    var doubles = new ArrayList<>(Arrays.asList("HTTP/0.9", "HTTP/1.0", "HTTP/1.1"));
    doubles.forEach((literal) -> {
      try {
        var actual = HttpVersion.get(literal).orElseThrow();
        assertNotNull(actual);
        assertEquals(literal, actual.LITTERAL);
      } catch (BadHttpVersionException e) {
        fail("failed to get EXACT match HttpVersion.");
      }
    });
  }

  @Test
  public void getVersion_successSingleExact() {
    var singles = new ArrayList<>(Arrays.asList("HTTP/2", "HTTP/3"));
    singles.forEach((literal) -> {
      try {
        var actual = HttpVersion.get(literal).orElseThrow();
        assertNotNull(actual);
        assertEquals(literal, actual.LITTERAL);
      } catch (BadHttpVersionException e) {
        fail("failed to get EXACT match HttpVersion.");
      }
    });
  }

  @Test
  public void getVersion_successPartial() {
    var partials = new ArrayList<List<String>>();
    partials.add(Arrays.asList("HTTP/0.1", "HTTP/0.9"));
    partials.add(Arrays.asList("HTTP/1.3", "HTTP/1.1"));

    partials.forEach((tuple) -> {
      try {
        var actual = HttpVersion.get(tuple.get(0)).orElseThrow();
        assertNotNull(actual);
        assertEquals(tuple.get(1), actual.LITTERAL);
      } catch (BadHttpVersionException e) {
        fail("failed to get EXACT match HttpVersion.");
      }
    });
  }
}
