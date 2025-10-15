/*
 * ESP32 - JSON API Server Example
 * 
 * This example demonstrates how to create a RESTful API server with:
 * - GET request handling with JSON response
 * - POST request handling with JSON data parsing
 * - Proper HTTP status codes and headers
 * 
 * Hardware: ESP32 Board
 * Library: DIYables_ESP32_WebServer, ArduinoJson
 */

#include <DIYables_ESP32_WebServer.h>
#include <ArduinoJson.h>

// WiFi credentials
const char WIFI_SSID[] = "YOUR_WIFI_SSID";
const char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";

// Create web server instance
DIYables_ESP32_WebServer server;

// JSON response templates
const char JSON_RESPONSE[] PROGMEM = R"rawliteral(
{
  "status": "success",
  "message": "Data received",
  "received_key": "%KEY%"
}
)rawliteral";

const char JSON_GET_RESPONSE[] PROGMEM = R"rawliteral(
{
  "status": "success",
  "message": "GET request received",
  "timestamp": %TIMESTAMP%
}
)rawliteral";

// API handlers
void handleApiData(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  Serial.print("[API] ");
  Serial.print(method);
  Serial.print(" request received");

  if (jsonData.length() > 0) {
    Serial.print(" with JSON: ");
    Serial.println(jsonData);
  } else {
    Serial.println();
  }

  if (method == "POST") {
    Serial.print("JSON Data Content: '");
    Serial.print(jsonData);
    Serial.println("'");

    if (jsonData.length() == 0) {
      Serial.println("Error: No JSON data received");
      client.println("HTTP/1.1 400 Bad Request");
      client.println("Content-Type: application/json");
      client.println("Connection: close");
      client.println();
      client.print("{\"status\": \"error\",\"message\": \"No JSON data received\"}");
      return;
    }

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, jsonData);
    if (!error) {
      const char* key = doc["key"] | "none";
      Serial.print("Successfully parsed JSON, key: ");
      Serial.println(key);
      String response = JSON_RESPONSE;
      response.replace("%KEY%", key);
      server.sendResponse(client, response.c_str(), "application/json");
    } else {
      Serial.print("JSON Parse Error: ");
      Serial.println(error.c_str());
      client.println("HTTP/1.1 400 Bad Request");
      client.println("Content-Type: application/json");
      client.println("Connection: close");
      client.println();
      client.print("{\"status\": \"error\",\"message\": \"Invalid JSON\"}");
    }
  } else if (method == "GET") {
    String response = JSON_GET_RESPONSE;
    response.replace("%TIMESTAMP%", String(millis()));
    server.sendResponse(client, response.c_str(), "application/json");
  } else {
    client.println("HTTP/1.1 405 Method Not Allowed");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.print("{\"status\":\"error\",\"message\":\"Method not allowed\"}");
  }
}

void handleNotFound(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  String response = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>404 Not Found</title></head><body>";
  response += "<h1>404 - Page Not Found</h1>";
  response += "<p>Method: " + method + "</p>";
  response += "<p>Sorry, we couldn't find that page!</p>";
  response += "<a href=\"/\">Return to Home</a></body></html>";
  server.sendResponse(client, response.c_str());
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("ESP32 - JSON API Server");

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure API routes
  server.addRoute("/api/data", handleApiData);

  // Start server
  server.begin();

  Serial.println("\n=== JSON API Server Ready! ===");
  Serial.print("Test GET: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/api/data");
  Serial.println("Test POST: Send JSON to same URL");
}

void loop() {
  server.handleClient();
}