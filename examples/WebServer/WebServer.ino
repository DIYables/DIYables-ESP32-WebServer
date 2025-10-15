/*
 * ESP32 - Multi-Page Web Server Example
 * 
 * This example demonstrates how to create a multi-page web server with:
 * - Home page with navigation
 * - Temperature monitoring page
 * - LED control page
 * 
 * Hardware: ESP32 Board
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
#define LED_PIN 2  // ESP32 built-in LED pin
int ledState = LOW;  // Track LED state

// Create web server instance
DIYables_ESP32_WebServer server;

// Helper function to send LED page with current status
void sendLedPage(WiFiClient& client) {
  String ledStatus = (ledState == HIGH) ? "ON" : "OFF";
  String response = LED_PAGE;
  response.replace("%LED_STATUS%", ledStatus);
  server.sendResponse(client, response.c_str());
}

// Page handlers

void handleHome(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  server.sendResponse(client, HOME_PAGE);
}

void handleTemperature(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  float tempC = 25.5;  // Simulated temperature value, you can replace with actual sensor reading
  
  String response = TEMPERATURE_PAGE;
  response.replace("%TEMP_C%", String(tempC, 1));
  
  server.sendResponse(client, response.c_str());
}

void handleLed(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  sendLedPage(client);
} 

void handleLedOn(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  ledState = HIGH;
  digitalWrite(LED_PIN, ledState);
  sendLedPage(client);
}

void handleLedOff(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  ledState = LOW;
  digitalWrite(LED_PIN, ledState);
  sendLedPage(client);
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);
  
  Serial.println("ESP32 - Multi-Page Web Server");
  
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
  
  // Configure routes
  server.addRoute("/", handleHome);
  server.addRoute("/temperature", handleTemperature);
  server.addRoute("/led", handleLed);
  server.addRoute("/led/on", handleLedOn);
  server.addRoute("/led/off", handleLedOff);
  
  // Start server
  server.begin();
  
  Serial.println("\n=== Web Server Ready! ===");
  Serial.print("Visit: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
}