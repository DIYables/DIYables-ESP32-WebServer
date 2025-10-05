#ifndef ESP32_WIFI_WEBSERVER_H
#define ESP32_WIFI_WEBSERVER_H

#include <WiFi.h>
#include "base64/Base64.h"

// Forward declare WebSocket class
class DIYables_ESP32_WebSocket;

#define MAX_ROUTES 10
#define MAX_PATH_LENGTH 32
#define MAX_HTML_SIZE 1024
#define MAX_QUERY_PARAMS 5
#define MAX_PARAM_KEY_LENGTH 16
#define MAX_PARAM_VALUE_LENGTH 16
#define MAX_AUTH_USERNAME_LENGTH 32
#define MAX_AUTH_PASSWORD_LENGTH 32
#define MAX_AUTH_REALM_LENGTH 64

// Structure to hold query parameters
struct QueryParams {
  struct Param {
    char key[MAX_PARAM_KEY_LENGTH];
    char value[MAX_PARAM_VALUE_LENGTH];
  };
  Param params[MAX_QUERY_PARAMS];
  int count;
};

// Handler function type
typedef void (*RouteHandler)(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData);

class DIYables_ESP32_WebServer {
public:
  DIYables_ESP32_WebServer(int port = 80);
  void begin();  // Start server assuming WiFi is already connected
  void begin(const char* ssid, const char* pass);  // Connect to WiFi and start server
  void addRoute(const char* path, RouteHandler handler);
  void setNotFoundHandler(RouteHandler handler);
  void handleClient();
  void sendResponse(WiFiClient& client, const char* content, const char* contentType = "text/html");
  void send404(WiFiClient& client);
  void printWifiStatus();
  
  // Basic Authentication methods (disabled by default for backward compatibility)
  void enableAuthentication(const char* username, const char* password, const char* realm = "Arduino Server");
  void disableAuthentication();
  bool isAuthenticationEnabled();
  void send401(WiFiClient& client);
  
  // WebSocket functionality
  DIYables_ESP32_WebSocket* enableWebSocket(uint16_t wsPort = 81);
  DIYables_ESP32_WebSocket* getWebSocket();
  void handleWebSocket();

private:
  WiFiServer server;
  DIYables_ESP32_WebSocket* webSocket;
  struct Route {
    char path[MAX_PATH_LENGTH];
    RouteHandler handler;
  };
  Route routes[MAX_ROUTES];
  int routeCount;
  RouteHandler notFoundHandler;
  
  // Authentication variables
  bool authEnabled;
  char authUsername[MAX_AUTH_USERNAME_LENGTH];
  char authPassword[MAX_AUTH_PASSWORD_LENGTH];
  char authRealm[MAX_AUTH_REALM_LENGTH];
  
  void parseQueryString(const String& path, QueryParams& params);
  void processRequest(WiFiClient& client, const String& method, const String& path, const QueryParams& params, const String& jsonData, const String& request);
  bool checkAuthentication(const String& request);
  String base64Encode(const String& input);
};

// Include WebSocket functionality (automatically available but only compiled if used)
#include "DIYables_ESP32_WebSocket.h"

// Beginner-friendly type aliases - no namespace needed!
using WebSocketServer = net::WebSocketServer;
using WebSocket = net::WebSocket;

#endif