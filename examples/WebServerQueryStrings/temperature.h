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
    }
    .temperature-value {
      font-size: 24px;
      font-weight: bold;
      color: #0000ff;
    }
    .unit-selector {
      margin: 20px 0;
    }
    .button {
      display: inline-block;
      padding: 10px 20px;
      margin: 5px;
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
  <h1>Temperature Data</h1>
  <div class="temperature-value">%TEMPERATURE_VALUE%</div>
  <div class="unit-selector">
    <p>Select temperature unit:</p>
    <a href="/temperature?unit=C" class="button">Celsius (°C)</a>
    <a href="/temperature?unit=F" class="button">Fahrenheit (°F)</a>
    <a href="/temperature?unit=K" class="button">Kelvin (K)</a>
  </div>
  <br><br>
  <a href="/">Back to Home</a>
  <br><br>
  <br><br>
  This works with Arduino Uno R4 WiFi and <a href="https://diyables.io/products/diyables-stem-v4-iot-fully-compatible-with-arduino-uno-r4-wifi" target="_blank">DIYables STEM V4 IoT</a>
</body>
</html>
)rawliteral";

#endif