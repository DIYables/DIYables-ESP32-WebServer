/*
 * ESP32 - Simple Web Server with Basic Authentication
 * 
 * This example demonstrates basic authentication using the DIYables_ESP32_WebServer library.
 * Adapted from the simple WiFi authentication example structure.
 * 
 * Hardware: ESP32
 * Library: DIYables_ESP32_WebServer (with Basic Authentication support)
 */

#include <DIYables_ESP32_WebServer.h>

// WiFi credentials
const char WIFI_SSID[] = "YOUR_WIFI_SSID";
const char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";

// Authentication credentials
const char* www_username = "admin";
const char* www_password = "arduino";

// Create web server instance
DIYables_ESP32_WebServer server;

// HTML page content
const char* LOGIN_SUCCESS_PAGE = R"(
<!DOCTYPE HTML>
<html>
<head><title>Arduino Uno R4 Web Server</title></head>
<body>
<h1>Login Successful!</h1>
<p>You are now logged in.</p>
<p>Server running with DIYables_ESP32_WebServer library</p>
</body>
</html>
)";

// Main page handler
void handleRoot(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  server.sendResponse(client, LOGIN_SUCCESS_PAGE);
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    Serial.println("Please upgrade the firmware");

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(WIFI_SSID);

  // Configure the main route
  server.addRoute("/", handleRoot);

  // Start server with WiFi connection (handles connection automatically)
  server.begin(WIFI_SSID, WIFI_PASSWORD);

  // Enable basic authentication
  server.enableAuthentication(www_username, www_password, "Arduino");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Server ready with authentication enabled");
  Serial.print("Username: ");
  Serial.println(www_username);
  Serial.print("Password: ");
  Serial.println(www_password);
}

void loop() {
  // Handle all client requests (including authentication)
  server.handleClient();
}
