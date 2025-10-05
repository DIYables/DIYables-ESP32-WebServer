#ifndef HOME_H
#define HOME_H

const char HOME_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="icon" href="data:,">
  <title>Arduino Web Server</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 50px;
      background-color: #f5f5f5;
    }
    .container {
      max-width: 600px;
      margin: 0 auto;
      background-color: white;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
    }
    h1 {
      color: #333;
      margin-bottom: 30px;
    }
    .nav-menu {
      list-style: none;
      padding: 0;
      margin: 0;
    }
    .nav-item {
      margin: 15px 0;
    }
    .nav-link {
      display: inline-block;
      padding: 15px 30px;
      text-decoration: none;
      background-color: #007bff;
      color: white;
      border-radius: 5px;
      font-size: 16px;
      min-width: 200px;
      transition: background-color 0.3s;
    }
    .nav-link:hover {
      background-color: #0056b3;
    }
    .description {
      margin: 30px 0;
      color: #666;
      font-size: 14px;
    }
    .features {
      margin: 20px 0;
      font-size: 12px;
      color: #888;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>Arduino Uno R4 WiFi Web Server</h1>
    <div class="description">
      <p>Welcome to the Arduino web server with query string support!</p>
      <p>Explore the interactive features below:</p>
    </div>
    <ul class="nav-menu">
      <li class="nav-item">
        <a href="/temperature" class="nav-link">🌡️ Temperature Data</a>
        <div class="features">Interactive unit selection (C, F, K)</div>
      </li>
      <li class="nav-item">
        <a href="/led" class="nav-link">💡 LED Control</a>
        <div class="features">Real-time status with query parameters</div>
      </li>
    </ul>
    <br><br><br><br>
    This works with Arduino Uno R4 WiFi and <a href="https://diyables.io/products/diyables-stem-v4-iot-fully-compatible-with-arduino-uno-r4-wifi" target="_blank">DIYables STEM V4 IoT</a>
  </div>
</body>
</html>
)rawliteral";

#endif