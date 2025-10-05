#include "DIYables_ESP32_WebServer.h"
#include "DIYables_ESP32_WebSocket.h"
#include "NotFound_Default.h"
#include "base64/Base64.h"

DIYables_ESP32_WebServer::DIYables_ESP32_WebServer(int port) : server(port), routeCount(0), notFoundHandler(nullptr), webSocket(nullptr), authEnabled(false) {
  // Initialize authentication variables
  memset(authUsername, 0, sizeof(authUsername));
  memset(authPassword, 0, sizeof(authPassword));
  strcpy(authRealm, "Arduino Server");
}

void DIYables_ESP32_WebServer::begin() {
  // Assume WiFi is already connected, just start the server
  Serial.print("Starting web server on IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void DIYables_ESP32_WebServer::begin(const char* ssid, const char* pass) {
  Serial.begin(9600);

  // Ensure clean WiFi state (prevents issues after code upload)
  WiFi.disconnect(true);
  delay(100);

  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());
  printWifiStatus();

  server.begin();
}

void DIYables_ESP32_WebServer::addRoute(const char* path, RouteHandler handler) {
  if (routeCount < MAX_ROUTES) {
    strncpy(routes[routeCount].path, path, MAX_PATH_LENGTH - 1);
    routes[routeCount].path[MAX_PATH_LENGTH - 1] = '\0';
    routes[routeCount].handler = handler;
    routeCount++;
  } else {
    Serial.println("Max routes reached!");
  }
}

void DIYables_ESP32_WebServer::setNotFoundHandler(RouteHandler handler) {
  notFoundHandler = handler;
}

void DIYables_ESP32_WebServer::parseQueryString(const String& path, QueryParams& params) {
  params.count = 0;
  int queryStart = path.indexOf('?');
  if (queryStart == -1) return;

  String query = path.substring(queryStart + 1);
  int start = 0;
  while (start < query.length() && params.count < MAX_QUERY_PARAMS) {
    int end = query.indexOf('&', start);
    if (end == -1) end = query.length();

    String pair = query.substring(start, end);
    int equals = pair.indexOf('=');
    if (equals != -1) {
      String key = pair.substring(0, equals);
      String value = pair.substring(equals + 1);
      strncpy(params.params[params.count].key, key.c_str(), MAX_PARAM_KEY_LENGTH - 1);
      params.params[params.count].key[MAX_PARAM_KEY_LENGTH - 1] = '\0';
      strncpy(params.params[params.count].value, value.c_str(), MAX_PARAM_VALUE_LENGTH - 1);
      params.params[params.count].value[MAX_PARAM_VALUE_LENGTH - 1] = '\0';
      params.count++;
    }
    start = end + 1;
  }
}

void DIYables_ESP32_WebServer::handleClient() {
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    String request = "";
    String method = "";
    bool currentLineIsBlank = true;
    bool isPost = false;
    bool headersComplete = false;
    int contentLength = 0;
    
    unsigned long requestStart = millis();
    const unsigned long REQUEST_TIMEOUT = 3000; // 3 second timeout

    while (client.connected() && (millis() - requestStart < REQUEST_TIMEOUT)) {
      if (client.available()) {
        char c = client.read();
        request += c;

        if (c == '\n' && currentLineIsBlank && !headersComplete) {
          headersComplete = true;
          
          // Parse first line of HTTP request
          String firstLine = request.substring(0, request.indexOf('\n'));
          int start = firstLine.indexOf(' ') + 1;
          int end = firstLine.indexOf(' ', start);
          String path = firstLine.substring(start, end);
          method = firstLine.substring(0, firstLine.indexOf(' '));

          // Check if it's a POST request
          isPost = (method == "POST");

          // Extract Content-Length for POST requests
          if (isPost) {
            int contentLengthIndex = request.indexOf("Content-Length: ");
            if (contentLengthIndex != -1) {
              int start = contentLengthIndex + 16;
              int end = request.indexOf('\r', start);
              String lengthStr = request.substring(start, end);
              contentLength = lengthStr.toInt();
              Serial.print("Content-Length: ");
              Serial.println(contentLength);
            }
          }

          // Parse query parameters
          QueryParams params;
          parseQueryString(path, params);

          // Extract path without query string for route matching
          if (path.indexOf('?') != -1) {
            path = path.substring(0, path.indexOf('?'));
          }

          // Debug: Print the requested path and method
          Serial.print("Method: ");
          Serial.println(method);
          Serial.print("Requested path: ");
          Serial.println(path);
          for (int i = 0; i < params.count; i++) {
            Serial.print("Query param: ");
            Serial.print(params.params[i].key);
            Serial.print("=");
            Serial.println(params.params[i].value);
          }

          // For POST requests, continue reading to get the body
          String jsonData = "";
          if (isPost && contentLength > 0) {
            // Continue reading the body
            continue;
          } else {
            // Process the request
            processRequest(client, method, path, params, jsonData, request);
            break;
          }
        } else if (headersComplete && isPost && contentLength > 0) {
          // We're reading the body now
          static String bodyData = "";
          bodyData += c;
          
          if (bodyData.length() >= contentLength) {
            String jsonData = bodyData;
            bodyData = ""; // Reset for next request
            
            // Parse first line of HTTP request again
            String firstLine = request.substring(0, request.indexOf('\n'));
            int start = firstLine.indexOf(' ') + 1;
            int end = firstLine.indexOf(' ', start);
            String path = firstLine.substring(start, end);
            
            // Parse query parameters
            QueryParams params;
            parseQueryString(path, params);
            
            // Extract path without query string for route matching
            if (path.indexOf('?') != -1) {
              path = path.substring(0, path.indexOf('?'));
            }
            
            Serial.print("JSON body: ");
            Serial.println(jsonData);
            
            processRequest(client, method, path, params, jsonData, request);
            break;
          }
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Client disconnected");
  }
}

void DIYables_ESP32_WebServer::processRequest(WiFiClient& client, const String& method, const String& path, const QueryParams& params, const String& jsonData, const String& request) {
  // Check authentication if enabled
  if (authEnabled && !checkAuthentication(request)) {
    send401(client);
    return;
  }
  
  // Find matching route
  bool routeFound = false;
  for (int i = 0; i < routeCount; i++) {
    if (path.equals(routes[i].path)) {
      routes[i].handler(client, method, String(""), params, jsonData);
      routeFound = true;
      break;
    }
  }

  if (!routeFound) {
    send404(client);
  }
}

void DIYables_ESP32_WebServer::sendResponse(WiFiClient& client, const char* content, const char* contentType) {
  client.println("HTTP/1.1 200 OK");
  client.print("Content-Type: ");
  client.println(contentType);
  client.println("Connection: close");
  client.println();
  client.print(content);
}

void DIYables_ESP32_WebServer::send404(WiFiClient& client) {
  QueryParams emptyParams;
  emptyParams.count = 0; // Explicitly initialize count
  String emptyJson = "";
  String emptyMethod = "";
  if (notFoundHandler != nullptr) {
    notFoundHandler(client, emptyMethod, String(""), emptyParams, emptyJson);
  } else {
	// send the default page
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.print(NOT_FOUND_PAGE_DEFAULT);
  }
}

void DIYables_ESP32_WebServer::printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}

// WebSocket functionality
DIYables_ESP32_WebSocket* DIYables_ESP32_WebServer::enableWebSocket(uint16_t wsPort) {
  Serial.println("=== Enabling WebSocket Server ===");
  Serial.print("Requested port: ");
  Serial.println(wsPort);
  
  if (webSocket == nullptr) {
    Serial.println("Creating new WebSocket instance...");
    webSocket = new DIYables_ESP32_WebSocket(wsPort);
    
    Serial.println("Starting WebSocket server...");
    if (webSocket->begin()) {
      Serial.println("✓ WebSocket server enabled successfully");
      return webSocket;
    } else {
      Serial.println("✗ Failed to start WebSocket server, cleaning up...");
      delete webSocket;
      webSocket = nullptr;
      return nullptr;
    }
  } else {
    Serial.println("WebSocket already exists, returning existing instance");
    return webSocket;
  }
}

DIYables_ESP32_WebSocket* DIYables_ESP32_WebServer::getWebSocket() {
  return webSocket;
}

void DIYables_ESP32_WebServer::handleWebSocket() {
  if (webSocket != nullptr) {
    webSocket->loop();
  }
}

// Authentication methods
void DIYables_ESP32_WebServer::enableAuthentication(const char* username, const char* password, const char* realm) {
  authEnabled = true;
  strncpy(authUsername, username, MAX_AUTH_USERNAME_LENGTH - 1);
  authUsername[MAX_AUTH_USERNAME_LENGTH - 1] = '\0';
  strncpy(authPassword, password, MAX_AUTH_PASSWORD_LENGTH - 1);
  authPassword[MAX_AUTH_PASSWORD_LENGTH - 1] = '\0';
  strncpy(authRealm, realm, MAX_AUTH_REALM_LENGTH - 1);
  authRealm[MAX_AUTH_REALM_LENGTH - 1] = '\0';
  
  Serial.println("Basic Authentication enabled");
  Serial.print("Realm: ");
  Serial.println(authRealm);
}

void DIYables_ESP32_WebServer::disableAuthentication() {
  authEnabled = false;
  Serial.println("Basic Authentication disabled");
}

bool DIYables_ESP32_WebServer::isAuthenticationEnabled() {
  return authEnabled;
}

void DIYables_ESP32_WebServer::send401(WiFiClient& client) {
  client.println("HTTP/1.1 401 Unauthorized");
  client.print("WWW-Authenticate: Basic realm=\"");
  client.print(authRealm);
  client.println("\"");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html><html><head><title>401 Unauthorized</title></head>");
  client.println("<body><h1>401 Unauthorized</h1>");
  client.println("<p>Access to this resource requires authentication.</p>");
  client.println("</body></html>");
}

bool DIYables_ESP32_WebServer::checkAuthentication(const String& request) {
  // Look for Authorization header
  int authIndex = request.indexOf("Authorization: Basic ");
  if (authIndex == -1) {
    return false; // No auth header found
  }
  
  // Extract the base64 encoded credentials
  int start = authIndex + 21; // "Authorization: Basic " length
  int end = request.indexOf('\r', start);
  if (end == -1) {
    end = request.indexOf('\n', start);
  }
  if (end == -1) {
    return false; // Malformed header
  }
  
  String encodedCredentials = request.substring(start, end);
  encodedCredentials.trim();
  
  // Create expected credentials string
  String expectedCredentials = String(authUsername) + ":" + String(authPassword);
  String expectedEncoded = base64Encode(expectedCredentials);
  
  // Compare credentials
  return encodedCredentials.equals(expectedEncoded);
}

String DIYables_ESP32_WebServer::base64Encode(const String& input) {
  // Use the existing Base64 library
  int inputLen = input.length();
  int outputLen = ((inputLen + 2) / 3) * 4;
  char* output = new char[outputLen + 1];
  
  base64_encode(output, (char*)input.c_str(), inputLen);
  output[outputLen] = '\0';
  
  String result = String(output);
  delete[] output;
  return result;
}

// WebSocket functionality temporarily disabled
// Will be re-enabled once properly implemented