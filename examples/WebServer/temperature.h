#ifndef TEMPERATURE_H
#define TEMPERATURE_H

const char TEMPERATURE_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="icon" href="data:,">
  <title>Temperature Data</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 50px;
      background-color: #f5f5f5;
    }
    .temperature-container {
      background: white;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
      display: inline-block;
      margin: 20px;
    }
    .temperature-value {
      font-size: 48px;
      font-weight: bold;
      color: #0066cc;
      margin: 20px 0;
    }
    .info {
      margin: 20px 0;
      font-size: 16px;
      color: #666;
    }
    .refresh-btn {
      background: #0066cc;
      color: white;
      padding: 10px 20px;
      text-decoration: none;
      border-radius: 5px;
      margin: 10px;
      display: inline-block;
    }
  </style>
</head>
<body>
  <h1>🌡️ Temperature Monitor</h1>
  
  <div class="temperature-container">
    <div class="temperature-value">%TEMP_C%°C</div>
    
    <div class="info">
      <p><strong>Status:</strong> Temperature reading</p>
      <p><em>Refresh the page to see updated values</em></p>
    </div>
    
    <a href="/temperature" class="refresh-btn">🔄 Refresh</a>
    <a href="/" class="refresh-btn">🏠 Home</a>
    <br><br><br><br>
    This works with Arduino Uno R4 WiFi and <a href="https://diyables.io/products/diyables-stem-v4-iot-fully-compatible-with-arduino-uno-r4-wifi" target="_blank">DIYables STEM V4 IoT</a>
  </div>
</body>
</html>
)rawliteral";

#endif