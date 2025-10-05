/*
 * Arduino Uno R4 WiFi - Query String Web Server Example
 * 
 * This example demonstrates how to create a web server with query parameters:
 * - Temperature page with unit conversion (C/F)
 * - LED control with query parameters
 * - Dynamic content based on URL parameters
 * 
 * Hardware: Arduino Uno R4 WiFi or DIYables STEM V4 IoT
 * Library: DIYables_ESP32_WebServer
 */

#include <DIYables_ESP32_WebServer.h>
#include "home.h"
#include "temperature.h"
#include "led.h"

// WiFi credentials
const char WIFI_SSID[] = "YOUR_WIFI_SSID";
const char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";

// LED configuration
#define LED_PIN 13
int ledState = LOW;  // Track LED state

// Create web server instance
DIYables_ESP32_WebServer server;

void handleHome(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  server.sendResponse(client, HOME_PAGE);
}

void handleTemperature(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  // Check for query parameter "unit"
  String unit = "C";
  for (int i = 0; i < params.count; i++) {
    if (String(params.params[i].key) == "unit") {
      unit = params.params[i].value;
    }
  }

  // Generate temperature display based on unit
  String temperatureDisplay = "Simulated temperature: 25°" + unit;

  // Use the TEMPERATURE_PAGE template and replace placeholder
  String response = TEMPERATURE_PAGE;
  response.replace("%TEMPERATURE_VALUE%", temperatureDisplay);
  
  server.sendResponse(client, response.c_str());
}

void handleLed(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  // Check for query parameter "state"
  String state = "";
  for (int i = 0; i < params.count; i++) {
    if (String(params.params[i].key) == "state") {
      state = params.params[i].value;
      break;
    }
  }
 
  // Control LED based on state
  if (state == "on") {
    ledState = HIGH;
    digitalWrite(LED_PIN, ledState);
  } else if (state == "off") {
    ledState = LOW;
    digitalWrite(LED_PIN, ledState);
  } else if (state == "") {
    // No state parameter provided, just show current status
  } else {
    // Invalid state parameter
    client.println("HTTP/1.1 400 Bad Request");
    client.println("Connection: close");
    client.println();
    client.print("Invalid state parameter. Use ?state=on or ?state=off");
    return;
  }

  // Get current LED state
  String ledStatus = (ledState == HIGH) ? "ON" : "OFF";

  // Use the LED_PAGE template and replace placeholders
  String response = LED_PAGE;
  response.replace("%LED_STATUS%", ledStatus);
  
  server.sendResponse(client, response.c_str());
}

void handleNotFound(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  String response = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>404 Not Found</title></head><body>";
  response += "<h1>404 - Page Not Found</h1>";
  response += "<p>Sorry, we couldn't find that page!</p>";
  response += "<a href=\"/\">Return to Home</a></body></html>";
  server.sendResponse(client, response.c_str());
}

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  pinMode(LED_PIN, OUTPUT);
  ledState = LOW; // Initialize state variable
  digitalWrite(LED_PIN, ledState); // Set LED to match state

  // Initialize web server with credentials from secrets.h
  server.begin(WIFI_SSID, WIFI_PASSWORD);

  // Explicitly print WiFi status (including IP address)
  server.printWifiStatus();

  // Add routes
  server.addRoute("/", handleHome);
  server.addRoute("/temperature", handleTemperature);
  server.addRoute("/led", handleLed);

  // Set custom 404 handler
  server.setNotFoundHandler(handleNotFound);
}

void loop() {
  server.handleClient();
}