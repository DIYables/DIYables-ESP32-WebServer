## ESP32  Web Server Library - ESP32_WebServer
This Arduino library provides an easy-to-use interface for creating **multi-page web servers** on the ESP32 board .  
It enables fast and reliable HTTP request handling, routing, and response generation for your web-based Arduino projects with **multiple interactive pages**.
* **Multi-page web server support** with unlimited page routing capabilities
* Simple routing system with support for GET, POST, and other HTTP methods
* Built-in query parameter parsing for dynamic content generation
* JSON data handling for modern web API development
* Template-based HTML responses with placeholder replacement for dynamic web content
* Default 404 error page provided, with support for custom error page handling
* **WebSocket support** for real-time bidirectional communication (built-in)
* **HTTP Basic Authentication** for secure access control (optional, disabled by default)
* Compatible with ESP32 board and [DIYables STEM V4 IoT](https://diyables.io/products/diyables-stem-v4-iot-fully-compatible-with-arduino-uno-r4-wifi)

With this library, you can easily create professional **multi-page web interfaces** and APIs for your ESP32  projects, allowing users to navigate between different pages seamlessly.

![ESP32 ](https://diyables.io/images/products/arduino-uno-r4-wifi-and-diyables-stem-v4-iot.jpg)



Product Link
----------------------------
* [ESP32](https://store.arduino.cc/products/uno-r4-wifi)
* [DIYables STEM V4 IoT](https://diyables.io/products/diyables-stem-v4-iot-fully-compatible-with-arduino-uno-r4-wifi)



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
* **WebServerWithAuthentication.ino**: Simple web server with **HTTP Basic Authentication** protection. Shows how to enable/disable authentication and secure your Arduino web server with username/password protection.



Tutorials
----------------------------
* [ESP32 - Web Server](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-web-server)
* [ESP32 - WebSocket](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-websocket)



References
----------------------------
* [ESP32 WebServer Library Reference](https://arduinogetstarted.com/reference/library/arduino-uno-r4-wifi-web-server-library)
* [Web Server Example](https://arduinogetstarted.com/reference/library/web-server-example)
* [Web Server Query Strings Example](https://arduinogetstarted.com/reference/library/web-server-query-string-example)
* [Web Server Json Example](https://arduinogetstarted.com/reference/library/web-server-json-example)
* [Web Server With Web Socket Example](https://arduinogetstarted.com/reference/library/web-server-with-websocket-example)
* [Web Server With Authentication Example](https://arduinogetstarted.com/reference/library/web-server-authentication-example)


Tested Hardware
----------------------------

| Board                   | Tested | Notes                                      |
|-------------------------|:------:|---------------------------------------------|
| ESP32     |   ✅   | Fully supported       |
| [DIYables STEM V4 IoT](https://diyables.io/products/diyables-stem-v4-iot-fully-compatible-with-arduino-uno-r4-wifi) |   ✅   | Fully supported          |
| [DIYables STEM V4 IoT Starter Kit](https://diyables.io/products/diyables-stem-v4-iot-starter-kit) |   ✅   | Fully supported          |

