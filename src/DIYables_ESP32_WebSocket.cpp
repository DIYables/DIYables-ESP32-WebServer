#include "DIYables_ESP32_WebSocket.h"

// Static instance pointer for callbacks
DIYables_ESP32_WebSocket* DIYables_ESP32_WebSocket::instance = nullptr;

DIYables_ESP32_WebSocket::DIYables_ESP32_WebSocket(uint16_t port) 
  : port(port), openHandler(nullptr), messageHandler(nullptr), closeHandler(nullptr), initialized(false), clientCount(0), lastWiFiCheck(0), wifiWasConnected(false) {
  wsServer = new net::WebSocketServer(port);
  instance = this; // Set static instance for callbacks
}

DIYables_ESP32_WebSocket::~DIYables_ESP32_WebSocket() {
  if (wsServer) {
    delete wsServer;
    wsServer = nullptr;
  }
  if (instance == this) {
    instance = nullptr;
  }
}

// Static callback functions
void DIYables_ESP32_WebSocket::staticOnConnection(net::WebSocket &ws) {
  if (instance) {
    Serial.print("WebSocket client connected from: ");
    Serial.println(ws.getRemoteIP());
    Serial.print("Total clients: ");
    Serial.println(instance->wsServer->countClients());
    
    // Set up individual client handlers
    ws.onMessage(staticOnMessage);
    ws.onClose(staticOnClose);
    
    if (instance->openHandler) {
      instance->openHandler(ws);
    }
  }
}

void DIYables_ESP32_WebSocket::staticOnMessage(net::WebSocket &ws, const net::WebSocket::DataType dataType, const char *message, uint16_t length) {
  if (instance && instance->messageHandler) {
    instance->messageHandler(ws, dataType, message, length);
  }
}

void DIYables_ESP32_WebSocket::staticOnClose(net::WebSocket &ws, const net::WebSocket::CloseCode code, const char *reason, uint16_t length) {
  if (instance) {
    Serial.print("WebSocket client disconnected - Code: ");
    Serial.print((int)code);
    Serial.print(", Remaining clients: ");
    Serial.println(instance->wsServer->countClients());
    
    if (instance->closeHandler) {
      instance->closeHandler(ws, code, reason, length);
    }
  }
}

bool DIYables_ESP32_WebSocket::begin() {
  Serial.println("=== WebSocket Server Initialization ===");
  
  if (!wsServer) {
    Serial.println("ERROR: WebSocket server not initialized!");
    return false;
  }
  
  // Check if WiFi is connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("ERROR: WiFi not connected! Cannot start WebSocket server.");
    Serial.print("WiFi Status: ");
    Serial.println(WiFi.status());
    return false;
  }
  
  Serial.print("WiFi connected, IP: ");
  Serial.println(WiFi.localIP());
  
  // Set up connection handler
  Serial.println("Setting up WebSocket connection handler...");
  wsServer->onConnection(staticOnConnection);
  
  // Start the WebSocket server
  Serial.println("Starting WebSocket server...");
  wsServer->begin();
  initialized = true;
  wifiWasConnected = true;
  lastWiFiCheck = millis();
  
  // Small delay to let server fully initialize
  delay(100);
  
  Serial.print("✓ WebSocket server started successfully on port ");
  Serial.println(port);
  Serial.print("✓ WebSocket URL: ws://");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(port);
  Serial.println("=== WebSocket Server Ready ===");
  
  return true;
}

void DIYables_ESP32_WebSocket::loop() {
  if (initialized && wsServer) {
    // Check WiFi connection every 2 seconds
    checkWiFiConnection();
    
    // Only listen if WiFi is connected
    if (WiFi.status() == WL_CONNECTED) {
      wsServer->listen();
    } else {
      static unsigned long lastWifiWarning = 0;
      if (millis() - lastWifiWarning > 5000) {
        Serial.println("WARNING: WiFi not connected, WebSocket not listening");
        lastWifiWarning = millis();
      }
    }
    
    // Debug: Periodically show WebSocket status and client count
    static unsigned long lastDebug = 0;
    if (millis() - lastDebug > 15000) { // Every 15 seconds
      Serial.println("=== WebSocket Status ===");
      Serial.print("Server initialized: ");
      Serial.println(initialized ? "YES" : "NO");
      Serial.print("WiFi connected: ");
      Serial.println(WiFi.status() == WL_CONNECTED ? "YES" : "NO");
      uint8_t actualClients = wsServer->countClients();
      Serial.print("Active clients: ");
      Serial.println(actualClients);
      Serial.print("Server listening on: ws://");
      Serial.print(WiFi.localIP());
      Serial.print(":");
      Serial.println(port);
      Serial.println("======================");
      lastDebug = millis();
    }
  } else {
    static unsigned long lastInitWarning = 0;
    if (millis() - lastInitWarning > 10000) {
      Serial.print("WARNING: WebSocket not initialized - initialized: ");
      Serial.print(initialized);
      Serial.print(", wsServer: ");
      Serial.println(wsServer != nullptr);
      lastInitWarning = millis();
    }
  }
}

void DIYables_ESP32_WebSocket::onOpen(WebSocketOpenHandler handler) {
  openHandler = handler;
}

void DIYables_ESP32_WebSocket::onMessage(WebSocketEventHandler handler) {
  messageHandler = handler;
}

void DIYables_ESP32_WebSocket::onClose(WebSocketCloseHandler handler) {
  closeHandler = handler;
}

void DIYables_ESP32_WebSocket::broadcastTXT(const char* payload) {
  if (wsServer && initialized) {
    wsServer->broadcast(net::WebSocket::DataType::TEXT, payload, strlen(payload));
  }
}

void DIYables_ESP32_WebSocket::broadcastTXT(const String& payload) {
  if (wsServer && initialized) {
    wsServer->broadcast(net::WebSocket::DataType::TEXT, payload.c_str(), payload.length());
  }
}

void DIYables_ESP32_WebSocket::broadcastBIN(const uint8_t* payload, size_t length) {
  if (wsServer && initialized) {
    wsServer->broadcast(net::WebSocket::DataType::BINARY, reinterpret_cast<const char*>(payload), length);
  }
}

uint8_t DIYables_ESP32_WebSocket::connectedClients() {
  if (wsServer && initialized) {
    return wsServer->countClients();
  }
  return 0;
}

bool DIYables_ESP32_WebSocket::isListening() {
  return initialized && wsServer;
}

void DIYables_ESP32_WebSocket::checkWiFiConnection() {
  // Check WiFi status every 2 seconds
  if (millis() - lastWiFiCheck > 2000) {
    lastWiFiCheck = millis();
    
    bool currentlyConnected = (WiFi.status() == WL_CONNECTED);
    
    // WiFi dropped - restart WebSocket
    if (wifiWasConnected && !currentlyConnected) {
      Serial.println("WiFi connection lost! Stopping WebSocket server...");
      wifiWasConnected = false;
      
      // Stop the WebSocket server
      if (wsServer && initialized) {
        initialized = false;
        delete wsServer;
        wsServer = new net::WebSocketServer(port);
      }
    }
    // WiFi reconnected - restart WebSocket
    else if (!wifiWasConnected && currentlyConnected) {
      Serial.println("WiFi reconnected! Restarting WebSocket server...");
      wifiWasConnected = true;
      restartWebSocket();
    }
  }
}

void DIYables_ESP32_WebSocket::restartWebSocket() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Cannot restart WebSocket - WiFi not connected");
    return;
  }
  
  Serial.println("Restarting WebSocket server...");
  
  // Stop existing server if running
  if (wsServer && initialized) {
    initialized = false;
    delete wsServer;
    wsServer = new net::WebSocketServer(port);
  }
  
  // Restart the server
  if (wsServer) {
    wsServer->onConnection(staticOnConnection);
    wsServer->begin();
    initialized = true;
    
    Serial.print("WebSocket server restarted on port ");
    Serial.println(port);
    Serial.print("WebSocket URL: ws://");
    Serial.print(WiFi.localIP());
    Serial.print(":");
    Serial.println(port);
  }
}
