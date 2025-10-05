## ESP32  Web Server Library - ESP32_WebServer
This  Web Server library provides an easy-to-use interface for creating **multi-page web servers** on the ESP32 board .  
It enables fast and reliable HTTP request handling, routing, and response generation for your web-based ESP32 projects with **multiple interactive pages**.
* **Multi-page web server support** with unlimited page routing capabilities
* Simple routing system with support for GET, POST, and other HTTP methods
* Built-in query parameter parsing for dynamic content generation
* JSON data handling for modern web API development
* Template-based HTML responses with placeholder replacement for dynamic web content
* Default 404 error page provided, with support for custom error page handling
* **WebSocket support** for real-time bidirectional communication (built-in)
* **HTTP Basic Authentication** for secure access control (optional, disabled by default)

With this library, you can easily create professional **multi-page web interfaces** and APIs for your ESP32  projects, allowing users to navigate between different pages seamlessly.



Features  
----------------------------  
* **Multi-page web server** with unlimited routing capabilities
* **WebSocket server support** with real-time bidirectional communication
* **HTTP Basic Authentication** for secure access control (optional, backward compatible)
* Simple HTTP server with routing capabilities
* Query string parameter parsing 
* JSON request/response handling
* Template-based HTML generation for dynamic web content
* Default 404 error page provided, with support for custom error page handling
* WiFi connection management
* Real-time status updates
* RESTful API support
* **WebSocket support** for real-time bidirectional communication



Available Examples
----------------------------
* **WebServer.ino**: **Multi-page web server** with routes for home, temperature, and LED control pages. Demonstrates fundamental routing and HTML template usage across multiple interconnected pages.
* **WebServerJson.ino**: Advanced JSON API server example. Shows how to handle POST requests with JSON data and return JSON responses for RESTful API development.
* **WebServerQueryStrings.ino**: Interactive **multi-page web server** demonstrating query parameter parsing. Features dynamic content generation based on URL parameters for temperature units and LED control with seamless page navigation.
* **WebServerWithWebSocket.ino**: Advanced **multi-page web server** with real-time WebSocket communication using the integrated library. Demonstrates both HTTP and WebSocket functionality for live data exchange and real-time control.
* **WebServerWithAuthentication.ino**: Simple web server with **HTTP Basic Authentication** protection. Shows how to enable/disable authentication and secure your ESP32 web server with username/password protection.



Tutorials
----------------------------
* [ESP32 - Web Server](https://esp32io.com/tutorials/esp32-web-server)
* [ESP32 - WebSocket](https://esp32io.com/tutorials/esp32-websocket)



References
----------------------------
* [ESP32 WebServer Library Reference](https://esp32io.com/reference/library/esp32-web-server-library)
* [Web Server Example](https://esp32io.com/reference/library/web-server-example)
* [Web Server Query Strings Example](https://esp32io.com/reference/library/web-server-query-string-example)
* [Web Server Json Example](https://esp32io.com/reference/library/web-server-json-example)
* [Web Server With Web Socket Example](https://esp32io.com/reference/library/web-server-with-websocket-example)
* [Web Server With Authentication Example](https://esp32io.com/reference/library/web-server-authentication-example)


Tested Hardware
----------------------------

| Board                   | Tested | Notes                                      |
|-------------------------|:------:|---------------------------------------------|
| ESP32     |   ✅   | Fully supported       |
| [DIYables ESP32 Development Board](https://diyables.io/products/38-pin-esp32s-esp-wroom-32-wifi-and-bluetooth-development-board-with-usb-type-c-and-cp2102-dual-core-esp32-microcontroller-for-iot-projects-compatible-with-arduino-ide) |   ✅   | Fully supported          |
| [DIYables ESP32 Starter Kit](https://diyables.io/products/esp32-starter-kit) |   ✅   | Fully supported          |

