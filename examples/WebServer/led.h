#ifndef LED_H
#define LED_H

const char LED_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="icon" href="data:,">
  <title>LED Control</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 50px;
    }
    .led-status {
      font-size: 24px;
      font-weight: bold;
      color: #0000ff;
    }
    .button {
      display: inline-block;
      padding: 10px 20px;
      margin: 10px;
      text-decoration: none;
      background-color: #007bff;
      color: white;
      border-radius: 5px;
      border: none;
      cursor: pointer;
    }
    .button:hover {
      background-color: #0056b3;
    }
  </style>
</head>
<body>
  <h1>LED Control Panel</h1>
  <div>LED Status: <span class="led-status">%LED_STATUS%</span></div>
  <p>Control the LED using the buttons below:</p>
  <a href="/led/on" class="button">Turn LED ON</a>
  <a href="/led/off" class="button">Turn LED OFF</a>
  <br><br>
  <a href="/">Back to Home</a>
  <br><br><br><br>
  This works with Arduino Uno R4 WiFi and <a href="https://diyables.io/products/diyables-stem-v4-iot-fully-compatible-with-arduino-uno-r4-wifi" target="_blank">DIYables STEM V4 IoT</a>
</body>
</html>
)rawliteral";

#endif