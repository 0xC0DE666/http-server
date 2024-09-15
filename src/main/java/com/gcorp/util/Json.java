package com.gcorp.util;

import java.io.IOException;

import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializationFeature;

public class Json {

  private static ObjectMapper mapper = defaultMapper();

  private static ObjectMapper defaultMapper() {
    var om = new ObjectMapper();
    om.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
    return om;
  }

  public static JsonNode parse(String src) throws IOException {
    return mapper.readTree(src);
  }

  public static JsonNode toJSON(Object obj) {
    return mapper.valueToTree(obj);
  }

  public static <T> T toPOJO(JsonNode node,  Class<T> clazz) throws IOException {
    return mapper.treeToValue(node, clazz);
  }

  public static String stringify(Object obj, boolean pretty) throws IOException {
    var writer = pretty ? mapper.writer().with(SerializationFeature.INDENT_OUTPUT) : mapper.writer();
    return writer.writeValueAsString(obj);
  }
}
